/* Copyright (C) 2013-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** @file CbmStsAlgoFindHits.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 03.05.2013
 **/
#include "CbmStsAlgoFindHits.h"

#include "CbmDigiManager.h"
#include "CbmStsCluster.h"
#include "CbmStsDigi.h"
#include "CbmStsHit.h"
#include "CbmStsParSensor.h"
#include "CbmStsParSensorCond.h"

#include <TGeoMatrix.h>
#include <TMath.h>

#include <iostream>

using std::pair;
using std::vector;


// -----   Constructor   ---------------------------------------------------
CbmStsAlgoFindHits::CbmStsAlgoFindHits() {}
// -------------------------------------------------------------------------


// -----   Create a new hit   ----------------------------------------------
void CbmStsAlgoFindHits::CreateHit(Double_t xLocal, Double_t yLocal, Double_t varX, Double_t varY, Double_t varXY,
                                   const CbmStsCluster& clusterF, const CbmStsCluster& clusterB, UInt_t indexF,
                                   UInt_t indexB, Double_t du, Double_t dv)
{

  // ---  Check output array
  assert(fHits);

  // --- Transform into global coordinate system
  Double_t local[3] = {xLocal, yLocal, 0.};
  Double_t global[3];
  if (fMatrix) fMatrix->LocalToMaster(local, global);
  else {
    global[0] = local[0];
    global[1] = local[1];
    global[2] = local[2];
  }  //? no transformation matrix available

  // We assume here that the local-to-global transformations is only translation
  // plus maybe rotation upside down or front-side back. In that case, the
  // global covariance matrix is the same as the local one.
  // TODO: Proper transformation of covariance matrix
  Double_t error[3] = {TMath::Sqrt(varX), TMath::Sqrt(varY), 0.};

  // --- Calculate hit time (average of cluster times)
  Double_t hitTime      = 0.5 * (clusterF.GetTime() + clusterB.GetTime());
  Double_t etF          = clusterF.GetTimeError();
  Double_t etB          = clusterB.GetTimeError();
  Double_t hitTimeError = 0.5 * TMath::Sqrt(etF * etF + etB * etB);

  // --- Create hit
  fHits->emplace_back(fAddress, global, error, varXY, indexF, indexB, hitTime, hitTimeError, du, dv);

  return;
}
// -------------------------------------------------------------------------


// -----   Algorithm execution   -------------------------------------------
Long64_t CbmStsAlgoFindHits::Exec(const vector<CbmStsCluster>& clustersF, const vector<CbmStsCluster>& clustersB,
                                  vector<CbmStsHit>& hits, UInt_t address, Double_t timeCutSig, Double_t timeCutAbs,
                                  Double_t dY, UInt_t nStrips, Double_t pitch, Double_t stereoF, Double_t stereoB,
                                  Double_t lorentzF, Double_t lorentzB, TGeoHMatrix* matrix)
{

  // Assert validity of parameters
  assert(nStrips > 0);
  assert(pitch > 0.);
  assert(TMath::Abs(stereoF - stereoB) > 0.1);

  // Set internal parameters
  fAddress   = address;
  fNofStrips = nStrips;
  fDy        = dY;
  fPitch     = pitch;
  fStereoF   = stereoF;
  fStereoB   = stereoB;
  fLorentzF  = lorentzF;
  fLorentzB  = lorentzB;
  fMatrix    = matrix;
  fHits      = &hits;
  fHits->clear();
  fTanStereoF = TMath::Tan(fStereoF * TMath::DegToRad());
  fTanStereoB = TMath::Tan(fStereoB * TMath::DegToRad());
  fErrorFac   = 1. / (fTanStereoB - fTanStereoF) / (fTanStereoB - fTanStereoF);
  fDx         = Double_t(fNofStrips) * fPitch;

  // Determine the maximum cluster time errors
  Double_t maxTerrF = 0.;
  for (auto& cluster : clustersF) {
    if (cluster.GetTimeError() > maxTerrF) maxTerrF = cluster.GetTimeError();
  }
  Double_t maxTerrB = 0.;
  for (auto& cluster : clustersB) {
    if (cluster.GetTimeError() > maxTerrB) maxTerrB = cluster.GetTimeError();
  }
  const Double_t maxTerrF2      = maxTerrF * maxTerrF;
  const Double_t maxTerrB2      = maxTerrB * maxTerrB;
  const Double_t max_sigma_both = 4. * TMath::Sqrt(maxTerrF2 + maxTerrB2);

  // --- Loop over front and back side clusters
  Long64_t nHits = 0;
  Int_t startB   = 0;
  for (UInt_t iClusterF = 0; iClusterF < clustersF.size(); iClusterF++) {
    const CbmStsCluster& clusterF = clustersF[iClusterF];

    Double_t tF        = clusterF.GetTime();
    Double_t tFerr2    = clusterF.GetTimeError() * clusterF.GetTimeError();
    Double_t max_sigma = 4. * TMath::Sqrt(tFerr2 + maxTerrB2);

    for (UInt_t iClusterB = startB; iClusterB < clustersB.size(); iClusterB++) {
      const CbmStsCluster& clusterB = clustersB[iClusterB];

      Double_t timeDiff = tF - clusterB.GetTime();
      if ((timeDiff > 0) && (timeDiff > max_sigma_both)) {
        startB++;
        continue;
      }
      else if ((timeDiff > 0) && (timeDiff > max_sigma)) {
        continue;
      }
      else if ((timeDiff < 0) && (fabs(timeDiff) > max_sigma))
        break;

      // Cut on time difference of the two clusters
      Double_t timeCut = -1.;
      if (timeCutAbs > 0.) timeCut = timeCutAbs;  // absolute cut value
      else {
        if (timeCutSig > 0.) {
          Double_t eF = clusterF.GetTimeError();
          Double_t eB = clusterB.GetTimeError();
          timeCut     = timeCutSig * TMath::Sqrt(eF * eF + eB * eB);
        }  //? cut calculated from cluster errors
      }
      if (fabs(clusterF.GetTime() - clusterB.GetTime()) > timeCut) continue;

      // --- Calculate intersection points
      Int_t nOfHits = IntersectClusters(clusterF, clusterB, iClusterF, iClusterB);
      nHits += nOfHits;

    }  //# clusters back side

  }  //# clusters front side

  return nHits;
}
// -------------------------------------------------------------------------


// -----   Get cluster position at read-out edge   -------------------------
void CbmStsAlgoFindHits::GetClusterPosition(Double_t centre, Double_t& xCluster, Int_t& side)
{

  // Take integer channel
  Int_t iChannel = Int_t(centre);
  Double_t xDif  = centre - Double_t(iChannel);

  // Calculate corresponding strip on sensor
  Int_t iStrip                 = -1;
  pair<Int_t, Int_t> stripSide = GetStrip(iChannel);
  iStrip                       = stripSide.first;
  side                         = stripSide.second;

  // Re-add difference to integer channel. Convert channel to
  // coordinate
  xCluster = (Double_t(iStrip) + xDif + 0.5) * fPitch;

  // Correct for Lorentz-Shift
  // Simplification: The correction uses only the y component of the
  // magnetic field. The shift is calculated using the mid-plane of the
  // sensor, which is not correct for tracks not traversing the entire
  // sensor thickness (i.e., are created or stopped somewhere in the sensor).
  // However, this is the best one can do in reconstruction.
  if (side == 0) xCluster -= fLorentzF;
  else
    xCluster -= fLorentzB;

  return;
}
// -------------------------------------------------------------------------


// -----   Get strip and side from channel number   ------------------------
pair<Int_t, Int_t> CbmStsAlgoFindHits::GetStrip(UInt_t channel) const
{

  Int_t stripNr = -1;
  Int_t side    = -1;

  // --- Determine front or back side
  if (channel < fNofStrips) {  // front side
    side    = 0;
    stripNr = channel;
  }
  else {  // back side
    side    = 1;
    stripNr = channel - fNofStrips;
  }

  // --- Horizontal cross-connection
  while (stripNr < 0)
    stripNr += fNofStrips;
  while (stripNr >= Int_t(fNofStrips))
    stripNr -= fNofStrips;

  return (pair<Int_t, Int_t>(stripNr, side));
}
// -------------------------------------------------------------------------


// -----   Intersection of two lines along the strips   --------------------
Bool_t CbmStsAlgoFindHits::Intersect(Double_t xF, Double_t exF, Double_t xB, Double_t exB, Double_t& x, Double_t& y,
                                     Double_t& varX, Double_t& varY, Double_t& varXY)
{

  // In the coordinate system with origin at the bottom left corner,
  // a line along the strips with coordinate x0 at the top edge is
  // given by the function y(x) = Dy - ( x - x0 ) / tan(phi), if
  // the stereo angle phi does not vanish. Two lines yF(x), yB(x) with top
  // edge coordinates xF, xB intersect at
  // x = ( tan(phiB)*xF - tan(phiF)*xB ) / (tan(phiB) - tan(phiF)
  // y = Dy + ( xB - xF ) / ( tan(phiB) - tan(phiF) )
  // For the case that one of the stereo angles vanish (vertical strips),
  // the calculation of the intersection is straightforward.

  // --- First check whether stereo angles are different. Else there is
  // --- no intersection.
  if (TMath::Abs(fStereoF - fStereoB) < 0.5) {
    x = -1000.;
    y = -1000.;
    return kFALSE;
  }

  // --- Now treat vertical front strips
  if (TMath::Abs(fStereoF) < 0.001) {
    x     = xF;
    y     = fDy - (xF - xB) / fTanStereoB;
    varX  = exF * exF;
    varY  = (exF * exF + exB * exB) / fTanStereoB / fTanStereoB;
    varXY = -1. * exF * exF / fTanStereoB;
    return IsInside(x - fDx / 2., y - fDy / 2.);
  }

  // --- Maybe the back side has vertical strips?
  if (TMath::Abs(fStereoB) < 0.001) {
    x     = xB;
    y     = fDy - (xB - xF) / fTanStereoF;
    varX  = exB * exB;
    varY  = (exF * exF + exB * exB) / fTanStereoF / fTanStereoF;
    varXY = -1. * exB * exB / fTanStereoF;
    return IsInside(x - fDx / 2., y - fDy / 2.);
  }

  // --- OK, both sides have stereo angle
  x     = (fTanStereoB * xF - fTanStereoF * xB) / (fTanStereoB - fTanStereoF);
  y     = fDy + (xB - xF) / (fTanStereoB - fTanStereoF);
  varX  = fErrorFac * (exF * exF * fTanStereoB * fTanStereoB + exB * exB * fTanStereoF * fTanStereoF);
  varY  = fErrorFac * (exF * exF + exB * exB);
  varXY = -1. * fErrorFac * (exF * exF * fTanStereoB + exB * exB * fTanStereoF);

  // --- Check for being in active area.
  return IsInside(x - fDx / 2., y - fDy / 2.);
}
// -------------------------------------------------------------------------


// -----   Intersect two clusters   ----------------------------------------
Int_t CbmStsAlgoFindHits::IntersectClusters(const CbmStsCluster& clusterF, const CbmStsCluster& clusterB, UInt_t indexF,
                                            UInt_t indexB)
{

  // --- Calculate cluster centre position on readout edge
  Int_t side  = -1;
  Double_t xF = -1.;
  Double_t xB = -1.;
  GetClusterPosition(clusterF.GetPosition(), xF, side);
  //std::cout << "Cluster position " << clusterF.GetPosition() << ": x "
  //    << xF << " side " << side;
  if (side != 0) {
    std::cout << "Cluster position " << clusterF.GetPosition() << ": x " << xF << " side " << side << std::endl;
  }
  assert(side == 0);
  Double_t exF = clusterF.GetPositionError() * fPitch;
  Double_t du  = exF * TMath::Cos(TMath::DegToRad() * fStereoF);
  GetClusterPosition(clusterB.GetPosition(), xB, side);
  assert(side == 1);
  Double_t exB = clusterB.GetPositionError() * fPitch;
  Double_t dv  = exB * TMath::Cos(TMath::DegToRad() * fStereoB);

  // --- Should be inside active area
  if (!(xF >= 0. || xF <= fDx)) return 0;
  if (!(xB >= 0. || xB <= fDx)) return 0;

  // --- Hit counter
  Int_t nHits = 0;

  // --- Calculate number of line segments due to horizontal
  // --- cross-connection. If x(y=0) does not fall on the bottom edge,
  // --- the strip is connected to the one corresponding to the line
  // --- with top edge coordinate xF' = xF +/- Dx. For odd combinations
  // --- of stereo angle and sensor dimensions, this could even happen
  // --- multiple times. For each of these lines, the intersection with
  // --- the line on the other side is calculated. If inside the active area,
  // --- a hit is created.
  Int_t nF = Int_t((xF + fDy * fTanStereoF) / fDx);
  Int_t nB = Int_t((xB + fDy * fTanStereoB) / fDx);

  // --- If n is positive, all lines from 0 to n must be considered,
  // --- if it is negative (phi negative), all lines from n to 0.
  Int_t nF1 = TMath::Min(0, nF);
  Int_t nF2 = TMath::Max(0, nF);
  Int_t nB1 = TMath::Min(0, nB);
  Int_t nB2 = TMath::Max(0, nB);

  // --- Double loop over possible lines
  Double_t xC    = -1.;  // x coordinate of intersection point
  Double_t yC    = -1.;  // y coordinate of intersection point
  Double_t varX  = 0.;   // variance of xC
  Double_t varY  = 0.;   // variance of yC
  Double_t varXY = 0.;   // covariance xC-yC
  for (Int_t iF = nF1; iF <= nF2; iF++) {
    Double_t xFi = xF - Double_t(iF) * fDx;
    for (Int_t iB = nB1; iB <= nB2; iB++) {
      Double_t xBi = xB - Double_t(iB) * fDx;

      // --- Intersect the two lines
      Bool_t found = Intersect(xFi, exF, xBi, exB, xC, yC, varX, varY, varXY);
      if (found) {

        // --- Transform into sensor system with origin at sensor centre
        xC -= 0.5 * fDx;
        yC -= 0.5 * fDy;
        // --- Create the hit
        CreateHit(xC, yC, varX, varY, varXY, clusterF, clusterB, indexF, indexB, du, dv);
        nHits++;

      }  //? Intersection of lines
    }    // lines on back side
  }      // lines on front side

  return nHits;

  return 0;
}
// -------------------------------------------------------------------------


// -----   Check whether a point is inside the active area   ---------------
Bool_t CbmStsAlgoFindHits::IsInside(Double_t x, Double_t y)
{
  if (x < -fDx / 2.) return kFALSE;
  if (x > fDx / 2.) return kFALSE;
  if (y < -fDy / 2.) return kFALSE;
  if (y > fDy / 2.) return kFALSE;
  return kTRUE;
}
// -------------------------------------------------------------------------


ClassImp(CbmStsAlgoFindHits)
