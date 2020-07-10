/** @file CbmStsAlgoFindHitsOrtho.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 03.05.2013
 **/
#include "CbmStsAlgoFindHitsOrtho.h"

#include <iostream>

#include "CbmStsCluster.h"
#include "CbmStsHit.h"
#include "CbmStsParSensor.h"
#include "CbmStsParSensorCond.h"
#include <TGeoMatrix.h>
#include <TMath.h>

#include "CbmDigiManager.h"
#include "CbmStsDigi.h"

using std::pair;
using std::vector;


// -----   Constructor   ---------------------------------------------------
CbmStsAlgoFindHitsOrtho::CbmStsAlgoFindHitsOrtho() {}
// -------------------------------------------------------------------------


// -----   Create a new hit   ----------------------------------------------
void CbmStsAlgoFindHitsOrtho::CreateHit(Double_t xLocal,
                                        Double_t yLocal,
                                        Double_t varX,
                                        Double_t varY,
                                        Double_t varXY,
                                        const CbmStsCluster& clusterF,
                                        const CbmStsCluster& clusterB,
                                        UInt_t indexF,
                                        UInt_t indexB,
                                        Double_t du,
                                        Double_t dv) {

  // ---  Check output array
  assert(fHits);

  // --- Transform into global coordinate system
  Double_t local[3] = {xLocal, yLocal, 0.};
  Double_t global[3];
  if (fMatrix)
    fMatrix->LocalToMaster(local, global);
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
  fHits->emplace_back(fAddress,
                      global,
                      error,
                      varXY,
                      indexF,
                      indexB,
                      hitTime,
                      hitTimeError,
                      du,
                      dv);

  return;
}
// -------------------------------------------------------------------------


// -----   Algorithm execution   -------------------------------------------
Long64_t CbmStsAlgoFindHitsOrtho::Exec(const vector<CbmStsCluster>& clustersF,
                                       const vector<CbmStsCluster>& clustersB,
                                       vector<CbmStsHit>& hits,
                                       UInt_t address,
                                       Double_t timeCutSig,
                                       Double_t timeCutAbs,
                                       UInt_t nStripsF,
                                       UInt_t nStripsB,
                                       Double_t pitchF,
                                       Double_t pitchB,
                                       Double_t lorentzF,
                                       Double_t lorentzB,
                                       TGeoHMatrix* matrix) {

  // Assert validity of parameters
  assert(nStripsF > 0);
  assert(nStripsB > 0);
  assert(pitchF > 0.);
  assert(pitchB > 0.);

  // Set internal parameters
  fAddress    = address;
  fNofStripsF = nStripsF;
  fNofStripsB = nStripsB;
  fPitchF     = pitchF;
  fPitchB     = pitchB;
  fLorentzF   = lorentzF;
  fLorentzB   = lorentzB;
  fMatrix     = matrix;
  fHits       = &hits;
  fHits->clear();
  fDx = Double_t(fNofStripsF) * fPitchF;
  fDy = Double_t(fNofStripsB) * fPitchB;

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
      } else if ((timeDiff > 0) && (timeDiff > max_sigma)) {
        continue;
      } else if ((timeDiff < 0) && (fabs(timeDiff) > max_sigma))
        break;

      // Cut on time difference of the two clusters
      Double_t timeCut = -1.;
      if (timeCutAbs > 0.)
        timeCut = timeCutAbs;  // absolute cut value
      else {
        if (timeCutSig > 0.) {
          Double_t eF = clusterF.GetTimeError();
          Double_t eB = clusterB.GetTimeError();
          timeCut     = timeCutSig * TMath::Sqrt(eF * eF + eB * eB);
        }  //? cut calculated from cluster errors
      }
      if (fabs(clusterF.GetTime() - clusterB.GetTime()) > timeCut) continue;

      // --- Calculate intersection points
      Int_t nOfHits =
        IntersectClusters(clusterF, clusterB, iClusterF, iClusterB);
      nHits += nOfHits;

    }  //# clusters back side

  }  //# clusters front side

  return nHits;
}
// -------------------------------------------------------------------------


// -----   Get cluster position at read-out edge   -------------------------
void CbmStsAlgoFindHitsOrtho::GetClusterPosition(Double_t centre,
                                                 Double_t& xCluster,
                                                 Int_t& side) {

  // Take integer channel
  Int_t iChannel = Int_t(centre);
  Double_t xDif  = centre - Double_t(iChannel);

  // Calculate corresponding strip on sensor
  Int_t iStrip                 = -1;
  pair<Int_t, Int_t> stripSide = GetStrip(iChannel);
  iStrip                       = stripSide.first;
  side                         = stripSide.second;
  assert(side == 0 || side == 1);

  // Re-add difference to integer channel. Convert channel to
  // coordinate
  if (side == 0)
    xCluster = (Double_t(iStrip) + xDif + 0.5) * fPitchF;
  else
    xCluster = (Double_t(iStrip) + xDif + 0.5) * fPitchB;

  // Correct for Lorentz-Shift
  // Simplification: The correction uses only the y component of the
  // magnetic field. The shift is calculated using the mid-plane of the
  // sensor, which is not correct for tracks not traversing the entire
  // sensor thickness (i.e., are created or stopped somewhere in the sensor).
  // However, this is the best one can do in reconstruction.
  if (side == 0)
    xCluster -= fLorentzF;
  else
    xCluster -= fLorentzB;

  return;
}
// -------------------------------------------------------------------------


// -----   Get strip and side from channel number   ------------------------
pair<Int_t, Int_t> CbmStsAlgoFindHitsOrtho::GetStrip(UInt_t channel) const {

  Int_t stripNr = -1;
  Int_t side    = -1;

  // --- Determine front or back side
  if (channel < fNofStripsF) {  // front side
    side    = 0;
    stripNr = channel;
  } else {  // back side
    side    = 1;
    stripNr = channel - fNofStripsF;
  }

  return (pair<Int_t, Int_t>(stripNr, side));
}
// -------------------------------------------------------------------------


// -----   Intersect two clusters   ----------------------------------------
Int_t CbmStsAlgoFindHitsOrtho::IntersectClusters(const CbmStsCluster& clusterF,
                                                 const CbmStsCluster& clusterB,
                                                 UInt_t indexF,
                                                 UInt_t indexB) {

  // --- Calculate cluster centre position on readout edge
  Int_t side  = -1;
  Double_t xF = -1.;
  Double_t xB = -1.;
  GetClusterPosition(clusterF.GetPosition(), xF, side);
  assert(side == 0);
  Double_t exF = clusterF.GetPositionError() * fPitchF;
  Double_t du  = exF;
  GetClusterPosition(clusterB.GetPosition(), xB, side);
  assert(side == 1);
  Double_t exB = clusterB.GetPositionError() * fPitchB;
  Double_t dv  = exB;

  // --- Should be inside active area
  if (!(xF >= 0. || xF <= fDx)) return 0;
  if (!(xB >= 0. || xB <= fDy)) return 0;

  // --- Hit counter
  Int_t nHits = 0;

  // In orthogonal sensor, all pairs of (front, back) cluster have
  // a single intersection
  // => exactly one hit!

  // --- Prepare hit coordinates and errors
  // In the coordinate system with origin at the bottom left corner,
  // the coordinates in the orthogonal sensor are straightforward.
  Double_t xC    = xF;         // x coordinate of intersection point
  Double_t yC    = xB;         // y coordinate of intersection point
  Double_t varX  = exF * exF;  // variance of xC
  Double_t varY  = exB * exB;  // variance of yC
  Double_t varXY = 0.;         // covariance xC-yC => independent variables!

  // --- Transform into sensor system with origin at sensor centre
  xC -= 0.5 * fDx;
  yC -= 0.5 * fDy;

  // --- Create the hit
  CreateHit(
    xC, yC, varX, varY, varXY, clusterF, clusterB, indexF, indexB, du, dv);
  nHits++;

  return nHits;
}
// -------------------------------------------------------------------------


// -----   Check whether a point is inside the active area   ---------------
Bool_t CbmStsAlgoFindHitsOrtho::IsInside(Double_t x, Double_t y) {
  if (x < -fDx / 2.) return kFALSE;
  if (x > fDx / 2.) return kFALSE;
  if (y < -fDy / 2.) return kFALSE;
  if (y > fDy / 2.) return kFALSE;
  return kTRUE;
}
// -------------------------------------------------------------------------


ClassImp(CbmStsAlgoFindHitsOrtho)
