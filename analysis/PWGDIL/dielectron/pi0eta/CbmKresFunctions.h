/**
 *    file CbmKresFunctions.h
 *
 *    author Ievgenii Kres
 *    date 04.04.2017
 *
 **/

#ifndef CBM_KRES_FUNCTIONS
#define CBM_KRES_FUNCTIONS

#include "CbmGlobalTrack.h"
#include "CbmLmvmKinematicParams.h"
#include "CbmMCTrack.h"
#include "CbmStsKFTrackFitter.h"
#include "CbmVertex.h"

#include "TLorentzVector.h"
#include "TMath.h"
#include "TMatrixTSym.h"
#include "TVector3.h"

#define M2E 2.6112004954086e-7
#define M2Pion 0.0194798351452

class CbmKresFunctions {
public:
  ///// Fit/refit track to one particular vector. Very useful if one combines two tracks in one mother particle and estimates possible decay point.
  static TVector3 FitToVertex(CbmStsTrack* stsTrack, double x, double y, double z)
  {
    CbmVertex* vtx      = new CbmVertex();
    TMatrixFSym* covMat = new TMatrixFSym(3);
    vtx->SetVertex(x, y, z, 0, 0, 0, *covMat);
    CbmStsKFTrackFitter fitter;
    FairTrackParam neu_track;
    fitter.FitToVertex(stsTrack, vtx, &neu_track);

    TVector3 momentum;

    neu_track.Momentum(momentum);

    delete vtx;
    delete covMat;

    return momentum;
  }

  ///// calculate only chi^2 of the fitting to the vertex
  static double ChiToVertex(CbmStsTrack* stsTrack, double x, double y, double z)
  {
    CbmVertex* vtx      = new CbmVertex();
    TMatrixFSym* covMat = new TMatrixFSym(3);
    vtx->SetVertex(x, y, z, 0, 0, 0, *covMat);
    CbmStsKFTrackFitter fitter;
    FairTrackParam neu_track;
    fitter.FitToVertex(stsTrack, vtx, &neu_track);

    double chi = fitter.GetChiToVertex(stsTrack, vtx);

    delete vtx;
    delete covMat;

    return chi;
  }


  ///// Fit/refit track to one particular vector and calculate only chi^2 of the fitting to the vertex
  static TVector3 FitToVertexAndGetChi(CbmStsTrack* stsTrack, double x, double y, double z, double& chi)
  {
    CbmVertex* vtx      = new CbmVertex();
    TMatrixFSym* covMat = new TMatrixFSym(3);
    vtx->SetVertex(x, y, z, 0, 0, 0, *covMat);
    CbmStsKFTrackFitter fitter;
    FairTrackParam neu_track;
    fitter.FitToVertex(stsTrack, vtx, &neu_track);

    chi = fitter.GetChiToVertex(stsTrack, vtx);

    TVector3 momentum;

    neu_track.Momentum(momentum);

    delete vtx;
    delete covMat;

    return momentum;
  }

  // calculation of invariant mass from 2 tracks using MCtrue momenta
  static double Invmass_2particles_MC(const CbmMCTrack* mctrack1, const CbmMCTrack* mctrack2)
  {
    TLorentzVector lorVec1;
    mctrack1->Get4Momentum(lorVec1);

    TLorentzVector lorVec2;
    mctrack2->Get4Momentum(lorVec2);

    TLorentzVector sum;
    sum = lorVec1 + lorVec2;

    return sum.Mag();
  }

  // calculation of invariant mass from 2 tracks using reconstructed momenta
  static double Invmass_2particles_RECO(const TVector3 part1, const TVector3 part2)
  {
    Double_t energy1 = TMath::Sqrt(part1.Mag2() + M2E);
    TLorentzVector lorVec1(part1, energy1);

    Double_t energy2 = TMath::Sqrt(part2.Mag2() + M2E);
    TLorentzVector lorVec2(part2, energy2);

    TLorentzVector sum;
    sum = lorVec1 + lorVec2;

    return sum.Mag();
  }

  // calculation of invariant mass from 4 tracks using MCtrue momenta
  static double Invmass_4particles_MC(const CbmMCTrack* mctrack1, const CbmMCTrack* mctrack2,
                                      const CbmMCTrack* mctrack3, const CbmMCTrack* mctrack4)
  {
    TLorentzVector lorVec1;
    mctrack1->Get4Momentum(lorVec1);

    TLorentzVector lorVec2;
    mctrack2->Get4Momentum(lorVec2);

    TLorentzVector lorVec3;
    mctrack3->Get4Momentum(lorVec3);

    TLorentzVector lorVec4;
    mctrack4->Get4Momentum(lorVec4);

    TLorentzVector sum;
    sum = lorVec1 + lorVec2 + lorVec3 + lorVec4;

    return sum.Mag();
  }

  // calculation of invariant mass from 4 tracks using reconstructed momenta
  static double Invmass_4particles_RECO(const TVector3 part1, const TVector3 part2, const TVector3 part3,
                                        const TVector3 part4)
  {
    Double_t energy1 = TMath::Sqrt(part1.Mag2() + M2E);
    TLorentzVector lorVec1(part1, energy1);

    Double_t energy2 = TMath::Sqrt(part2.Mag2() + M2E);
    TLorentzVector lorVec2(part2, energy2);

    Double_t energy3 = TMath::Sqrt(part3.Mag2() + M2E);
    TLorentzVector lorVec3(part3, energy3);

    Double_t energy4 = TMath::Sqrt(part4.Mag2() + M2E);
    TLorentzVector lorVec4(part4, energy4);

    TLorentzVector sum;
    sum = lorVec1 + lorVec2 + lorVec3 + lorVec4;

    return sum.Mag();
  }

  // gives amount of daughter particles belonging to current particle -> MCtrue info
  static int NofDaughters(int motherId, vector<CbmMCTrack*> MC)
  {
    int nofDaughters = 0;
    for (size_t i = 0; i < MC.size(); i++) {
      int motherId_temp = MC.at(i)->GetMotherId();
      if (motherId == motherId_temp) nofDaughters++;
    }
    return nofDaughters;
  }


  // calculation of many interesting for analysis physics parameters of 2 tracks: Invariant mass, opening angle, rapidity, pt,
  static CbmLmvmKinematicParams CalculateKinematicParamsReco(const TVector3 electron1, const TVector3 electron2)
  {
    CbmLmvmKinematicParams params;

    Double_t energyP = TMath::Sqrt(electron1.Mag2() + M2E);
    TLorentzVector lorVecP(electron1, energyP);

    Double_t energyM = TMath::Sqrt(electron2.Mag2() + M2E);
    TLorentzVector lorVecM(electron2, energyM);

    TVector3 momPair    = electron1 + electron2;
    Double_t energyPair = energyP + energyM;
    Double_t ptPair     = momPair.Perp();
    Double_t pzPair     = momPair.Pz();
    Double_t yPair      = 0.5 * TMath::Log((energyPair + pzPair) / (energyPair - pzPair));
    Double_t anglePair  = lorVecM.Angle(lorVecP.Vect());
    Double_t theta      = 180. * anglePair / TMath::Pi();
    Double_t minv       = 2. * TMath::Sin(anglePair / 2.) * TMath::Sqrt(electron1.Mag() * electron2.Mag());

    params.fMomentumMag = momPair.Mag();
    params.fPt          = ptPair;
    params.fRapidity    = yPair;
    params.fMinv        = minv;
    params.fAngle       = theta;
    return params;
  }


  // calculation of many interesting for analysis physics parameters of 4 tracks: Invariant mass, opening angle, rapidity, pt,
  static CbmLmvmKinematicParams CalculateKinematicParams_4particles(const TVector3 part1, const TVector3 part2,
                                                                    const TVector3 part3, const TVector3 part4)
  {
    CbmLmvmKinematicParams params;

    Double_t energy1 = TMath::Sqrt(part1.Mag2() + M2E);
    TLorentzVector lorVec1(part1, energy1);

    Double_t energy2 = TMath::Sqrt(part2.Mag2() + M2E);
    TLorentzVector lorVec2(part2, energy2);

    Double_t energy3 = TMath::Sqrt(part3.Mag2() + M2E);
    TLorentzVector lorVec3(part3, energy3);

    Double_t energy4 = TMath::Sqrt(part4.Mag2() + M2E);
    TLorentzVector lorVec4(part4, energy4);

    TLorentzVector sum;
    sum = lorVec1 + lorVec2 + lorVec3 + lorVec4;

    TVector3 momPair    = part1 + part2 + part3 + part4;
    Double_t energyPair = energy1 + energy2 + energy3 + energy4;
    Double_t ptPair     = momPair.Perp();
    Double_t pzPair     = momPair.Pz();
    Double_t yPair      = 0.5 * TMath::Log((energyPair + pzPair) / (energyPair - pzPair));
    Double_t minv       = sum.Mag();

    params.fMomentumMag = momPair.Mag();
    params.fPt          = ptPair;
    params.fRapidity    = yPair;
    params.fMinv        = minv;
    params.fAngle       = 0;
    return params;
  }


  // calculation of opening angle from 2 tracks using reconstructed momenta
  static Double_t CalculateOpeningAngle_Reco(TVector3 electron1, TVector3 electron2)
  {
    Double_t energyP = TMath::Sqrt(electron1.Mag2() + M2E);
    TLorentzVector lorVecP(electron1, energyP);

    Double_t energyM = TMath::Sqrt(electron2.Mag2() + M2E);
    TLorentzVector lorVecM(electron2, energyM);

    Double_t anglePair = lorVecM.Angle(lorVecP.Vect());
    Double_t theta     = 180. * anglePair / TMath::Pi();

    return theta;
  }


  // calculation of opening angle from 2 tracks using MCtrue momenta
  static Double_t CalculateOpeningAngle_MC(CbmMCTrack* mctrack1, CbmMCTrack* mctrack2)
  {
    TVector3 electron1;
    mctrack1->GetMomentum(electron1);
    Double_t energyP = TMath::Sqrt(electron1.Mag2() + M2E);
    TLorentzVector lorVecP(electron1, energyP);

    TVector3 electron2;
    mctrack2->GetMomentum(electron2);
    Double_t energyM = TMath::Sqrt(electron2.Mag2() + M2E);
    TLorentzVector lorVecM(electron2, energyM);

    Double_t anglePair = lorVecM.Angle(lorVecP.Vect());
    Double_t theta     = 180. * anglePair / TMath::Pi();

    return theta;
  }


  // calculation of opening angle from 2 photons using reconstructed momenta
  static Double_t CalculateOpeningAngleBetweenGammas_Reco(TVector3 electron1, TVector3 electron2, TVector3 electron3,
                                                          TVector3 electron4)
  {
    Double_t energy1 = TMath::Sqrt(electron1.Mag2() + M2E);
    TLorentzVector lorVec1(electron1, energy1);

    Double_t energy2 = TMath::Sqrt(electron2.Mag2() + M2E);
    TLorentzVector lorVec2(electron2, energy2);

    Double_t energy3 = TMath::Sqrt(electron3.Mag2() + M2E);
    TLorentzVector lorVec3(electron3, energy3);

    Double_t energy4 = TMath::Sqrt(electron4.Mag2() + M2E);
    TLorentzVector lorVec4(electron4, energy4);

    TLorentzVector lorPhoton1 = lorVec1 + lorVec2;
    TLorentzVector lorPhoton2 = lorVec3 + lorVec4;

    Double_t angleBetweenPhotons = lorPhoton1.Angle(lorPhoton2.Vect());
    Double_t theta               = 180. * angleBetweenPhotons / TMath::Pi();

    return theta;
  }


  // calculation of invariant mass of 4 particles using reconstructed value of momenta
  static double Invmass_2el_2pions_RECO(const TVector3 part1El, const TVector3 part2El, const TVector3 part3Pion,
                                        const TVector3 part4Pion)
  {
    Double_t energy1 = TMath::Sqrt(part1El.Mag2() + M2E);
    TLorentzVector lorVec1(part1El, energy1);

    Double_t energy2 = TMath::Sqrt(part2El.Mag2() + M2E);
    TLorentzVector lorVec2(part2El, energy2);

    Double_t energy3 = TMath::Sqrt(part3Pion.Mag2() + M2Pion);
    TLorentzVector lorVec3(part3Pion, energy3);

    Double_t energy4 = TMath::Sqrt(part4Pion.Mag2() + M2Pion);
    TLorentzVector lorVec4(part4Pion, energy4);

    TLorentzVector sum;
    sum = lorVec1 + lorVec2 + lorVec3 + lorVec4;

    return sum.Mag();
  }


  // calculation of invariant mass of 6 particles using true value of momenta
  static double Invmass_6particles_MC(const CbmMCTrack* mctrack1, const CbmMCTrack* mctrack2,
                                      const CbmMCTrack* mctrack3, const CbmMCTrack* mctrack4,
                                      const CbmMCTrack* mctrack5, const CbmMCTrack* mctrack6)
  {
    TLorentzVector lorVec1;
    mctrack1->Get4Momentum(lorVec1);

    TLorentzVector lorVec2;
    mctrack2->Get4Momentum(lorVec2);

    TLorentzVector lorVec3;
    mctrack3->Get4Momentum(lorVec3);

    TLorentzVector lorVec4;
    mctrack4->Get4Momentum(lorVec4);

    TLorentzVector lorVec5;
    mctrack5->Get4Momentum(lorVec5);

    TLorentzVector lorVec6;
    mctrack6->Get4Momentum(lorVec6);

    TLorentzVector sum;
    sum = lorVec1 + lorVec2 + lorVec3 + lorVec4 + lorVec5 + lorVec6;

    return sum.Mag();
  }

  // calculation of invariant mass of 6 particles using reconstructed value of momenta
  static double Invmass_4el_2pions_RECO(const TVector3 part1El, const TVector3 part2El, const TVector3 part3El,
                                        const TVector3 part4El, const TVector3 part5Pion, const TVector3 part6Pion)
  {
    Double_t energy1 = TMath::Sqrt(part1El.Mag2() + M2E);
    TLorentzVector lorVec1(part1El, energy1);

    Double_t energy2 = TMath::Sqrt(part2El.Mag2() + M2E);
    TLorentzVector lorVec2(part2El, energy2);

    Double_t energy3 = TMath::Sqrt(part3El.Mag2() + M2E);
    TLorentzVector lorVec3(part3El, energy3);

    Double_t energy4 = TMath::Sqrt(part4El.Mag2() + M2E);
    TLorentzVector lorVec4(part4El, energy4);

    Double_t energy5 = TMath::Sqrt(part5Pion.Mag2() + M2Pion);
    TLorentzVector lorVec5(part5Pion, energy5);

    Double_t energy6 = TMath::Sqrt(part6Pion.Mag2() + M2Pion);
    TLorentzVector lorVec6(part6Pion, energy6);

    TLorentzVector sum;
    sum = lorVec1 + lorVec2 + lorVec3 + lorVec4 + lorVec5 + lorVec6;

    return sum.Mag();
  }


  /// calculate opening angle between two pions using reconstructed momenta
  static Double_t CalculateOpeningAngleBetweenPions_Reco(TVector3 electron1, TVector3 electron2)
  {
    Double_t energyP = TMath::Sqrt(electron1.Mag2() + M2Pion);
    TLorentzVector lorVecP(electron1, energyP);

    Double_t energyM = TMath::Sqrt(electron2.Mag2() + M2Pion);
    TLorentzVector lorVecM(electron2, energyM);

    Double_t anglePair = lorVecM.Angle(lorVecP.Vect());
    Double_t theta     = 180. * anglePair / TMath::Pi();

    return theta;
  }


  /// calculate opening angle between two pions using MCtrue momenta
  static Double_t CalculateOpeningAngleBetweenPions_MC(CbmMCTrack* mctrack1, CbmMCTrack* mctrack2)
  {
    TVector3 electron1;
    mctrack1->GetMomentum(electron1);
    Double_t energyP = TMath::Sqrt(electron1.Mag2() + M2Pion);
    TLorentzVector lorVecP(electron1, energyP);

    TVector3 electron2;
    mctrack2->GetMomentum(electron2);
    Double_t energyM = TMath::Sqrt(electron2.Mag2() + M2Pion);
    TLorentzVector lorVecM(electron2, energyM);

    Double_t anglePair = lorVecM.Angle(lorVecP.Vect());
    Double_t theta     = 180. * anglePair / TMath::Pi();

    return theta;
  }


  // calculation of many interesting for analysis physics parameters of 2 leptons and 2 pions: Invariant mass, opening angle, rapidity, pt,
  static CbmLmvmKinematicParams CalculateKinematicParams_2el_2pions(const TVector3 part1, const TVector3 part2,
                                                                    const TVector3 part3, const TVector3 part4)
  {
    CbmLmvmKinematicParams params;

    Double_t energy1 = TMath::Sqrt(part1.Mag2() + M2E);
    TLorentzVector lorVec1(part1, energy1);

    Double_t energy2 = TMath::Sqrt(part2.Mag2() + M2E);
    TLorentzVector lorVec2(part2, energy2);

    Double_t energy3 = TMath::Sqrt(part3.Mag2() + M2Pion);
    TLorentzVector lorVec3(part3, energy3);

    Double_t energy4 = TMath::Sqrt(part4.Mag2() + M2Pion);
    TLorentzVector lorVec4(part4, energy4);

    TLorentzVector sum;
    sum = lorVec1 + lorVec2 + lorVec3 + lorVec4;

    TVector3 momPair    = part1 + part2 + part3 + part4;
    Double_t energyPair = energy1 + energy2 + energy3 + energy4;
    Double_t ptPair     = momPair.Perp();
    Double_t pzPair     = momPair.Pz();
    Double_t yPair      = 0.5 * TMath::Log((energyPair + pzPair) / (energyPair - pzPair));
    Double_t minv       = sum.Mag();

    params.fMomentumMag = momPair.Mag();
    params.fPt          = ptPair;
    params.fRapidity    = yPair;
    params.fMinv        = minv;
    params.fAngle       = 0;
    return params;
  }
};

#endif
