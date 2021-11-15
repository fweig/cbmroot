/* Copyright (C) 2015-2021 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Elena Lebedeva [committer], Semen Lebedev */

#ifndef LMVM_KINE_PAR_H
#define LMVM_KINE_PAR_H

#include "CbmMCTrack.h"

#include "TLorentzVector.h"
#include "TMath.h"

#include "LmvmCand.h"

#define M2E 2.6112004954086e-7

class LmvmKinePar {
public:
  Double_t fMomentumMag = 0.;  // Absolute value of momentum
  Double_t fPt          = 0.;  // Transverse momentum
  Double_t fRapidity    = 0.;  // Rapidity
  Double_t fMinv        = 0.;  // Invariant mass
  Double_t fAngle       = 0.;  // Opening angle

  /*
    * Calculate kinematic parameters for MC tracks.
    */
  static LmvmKinePar Create(const CbmMCTrack* mctrackP, const CbmMCTrack* mctrackM)
  {
    LmvmKinePar params;
    if (mctrackP == nullptr || mctrackM == nullptr) return params;

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
    Double_t yPair      = 0.5 * TMath::Log((energyPair + pzPair) / (energyPair - pzPair));
    Double_t anglePair  = lorVecM.Angle(lorVecP.Vect());
    Double_t theta      = 180. * anglePair / TMath::Pi();
    Double_t minv       = 2. * TMath::Sin(anglePair / 2.) * TMath::Sqrt(momM.Mag() * momP.Mag());

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
  static LmvmKinePar Create(const LmvmCand* candP, const LmvmCand* candM)
  {
    LmvmKinePar params;
    if (candP == nullptr || candM == nullptr) return params;

    TLorentzVector lorVecP(candP->fMomentum, candP->fEnergy);
    TLorentzVector lorVecM(candM->fMomentum, candM->fEnergy);

    TVector3 momPair    = candP->fMomentum + candM->fMomentum;
    Double_t energyPair = candP->fEnergy + candM->fEnergy;
    Double_t ptPair     = momPair.Perp();
    Double_t pzPair     = momPair.Pz();
    Double_t yPair      = 0.5 * TMath::Log((energyPair + pzPair) / (energyPair - pzPair));
    Double_t anglePair  = lorVecM.Angle(lorVecP.Vect());
    Double_t theta      = 180. * anglePair / TMath::Pi();
    Double_t minv = 2. * TMath::Sin(anglePair / 2.) * TMath::Sqrt(candM->fMomentum.Mag() * candP->fMomentum.Mag());

    params.fMomentumMag = momPair.Mag();
    params.fPt          = ptPair;
    params.fRapidity    = yPair;
    params.fMinv        = minv;
    params.fAngle       = theta;
    return params;
  }
};

#endif
