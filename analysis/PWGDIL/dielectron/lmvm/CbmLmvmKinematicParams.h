/**
 * @author Elena Lebedeva <e.lebedeva@gsi.de>
 * @since 2015
 * @version 1.0
 **/

#ifndef CBM_LMVM_KINEMATIC_PARAMS_H
#define CBM_LMVM_KINEMATIC_PARAMS_H

#include "CbmLmvmCandidate.h"
#include "CbmMCTrack.h"
#include "TLorentzVector.h"
#include "TMath.h"

#define M2E 2.6112004954086e-7

class CbmLmvmKinematicParams {
public:
  Double_t fMomentumMag;  // Absolute value of momentum
  Double_t fPt;           // Transverse momentum
  Double_t fRapidity;     // Rapidity
  Double_t fMinv;         // Invariant mass
  Double_t fAngle;        // Opening angle

  /*
    * Calculate kinematic parameters for MC tracks.
    */
  static CbmLmvmKinematicParams
  KinematicParamsWithMcTracks(const CbmMCTrack* mctrackP,
                              const CbmMCTrack* mctrackM) {
    CbmLmvmKinematicParams params;

    TVector3 momP;  //momentum e+
    mctrackP->GetMomentum(momP);
    Double_t energyP = TMath::Sqrt(momP.Mag2() + M2E);
    TLorentzVector lorVecP(momP, energyP);

    TVector3 momM;  //momentum e-
    mctrackM->GetMomentum(momM);
    Double_t energyM = TMath::Sqrt(momM.Mag2() + M2E);
    TLorentzVector lorVecM(momM, energyM);

    TVector3 momPair    = momP + momM;
    Double_t energyPair = energyP + energyM;
    Double_t ptPair     = momPair.Perp();
    Double_t pzPair     = momPair.Pz();
    Double_t yPair =
      0.5 * TMath::Log((energyPair + pzPair) / (energyPair - pzPair));
    Double_t anglePair = lorVecM.Angle(lorVecP.Vect());
    Double_t theta     = 180. * anglePair / TMath::Pi();
    Double_t minv =
      2. * TMath::Sin(anglePair / 2.) * TMath::Sqrt(momM.Mag() * momP.Mag());

    params.fMomentumMag = momPair.Mag();
    params.fPt          = ptPair;
    params.fRapidity    = yPair;
    params.fMinv        = minv;
    params.fAngle       = theta;
    return params;
  }

  /*
    * Calculate kinematic parameters for LMVM candidates.
    */
  static CbmLmvmKinematicParams
  KinematicParamsWithCandidates(const CbmLmvmCandidate* candP,
                                const CbmLmvmCandidate* candM) {
    CbmLmvmKinematicParams params;

    TLorentzVector lorVecP(candP->fMomentum, candP->fEnergy);
    TLorentzVector lorVecM(candM->fMomentum, candM->fEnergy);

    TVector3 momPair    = candP->fMomentum + candM->fMomentum;
    Double_t energyPair = candP->fEnergy + candM->fEnergy;
    Double_t ptPair     = momPair.Perp();
    Double_t pzPair     = momPair.Pz();
    Double_t yPair =
      0.5 * TMath::Log((energyPair + pzPair) / (energyPair - pzPair));
    Double_t anglePair = lorVecM.Angle(lorVecP.Vect());
    Double_t theta     = 180. * anglePair / TMath::Pi();
    Double_t minv =
      2. * TMath::Sin(anglePair / 2.)
      * TMath::Sqrt(candM->fMomentum.Mag() * candP->fMomentum.Mag());

    params.fMomentumMag = momPair.Mag();
    params.fPt          = ptPair;
    params.fRapidity    = yPair;
    params.fMinv        = minv;
    params.fAngle       = theta;
    return params;
  }
};

#endif
