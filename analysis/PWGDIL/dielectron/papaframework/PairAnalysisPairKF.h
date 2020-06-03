#ifndef PAIRANALYSISPAIRKF_H
#define PAIRANALYSISPAIRKF_H
/* Copyright(c) 1998-2009, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

//#############################################################
//#                                                           #
//#                  PairAnalysisPairKF                      #
//#               Class to store pair information             #
//#                                                           #
//#                                                           #
//#  Authors:                                                 #
//#   Julian    Book,     Uni Ffm / Julian.Book@cern.ch       #
//#                                                           #
//#############################################################

#include <TMath.h>
#include <TLorentzVector.h>
#include <KFParticle.h>

#include "PairAnalysisTrack.h"
#include "PairAnalysisTrackRotator.h"
#include "PairAnalysisPair.h"

class CbmVertex;
class CbmMCTrack;

class PairAnalysisPairKF : public PairAnalysisPair {
public:
  PairAnalysisPairKF();
  virtual ~PairAnalysisPairKF();
  PairAnalysisPairKF(const PairAnalysisPair& pair);

  PairAnalysisPairKF(PairAnalysisTrack * const particle1, Int_t pid1,
		     PairAnalysisTrack * const particle2, Int_t pid2, Char_t type);

  void SetTracks(PairAnalysisTrack * const particle1, Int_t pid1,
                 PairAnalysisTrack * const particle2, Int_t pid2);

  void SetMCTracks(const CbmMCTrack * const particle1,
		   const CbmMCTrack * const particle2);

  // kinematics
  virtual Double_t Px() const { return fPair.GetPx(); }
  virtual Double_t Py() const { return fPair.GetPy(); }
  virtual Double_t Pz() const { return fPair.GetPz(); }
  virtual Double_t Pt() const { return fPair.GetPt(); }
  virtual Double_t P() const  { return fPair.GetP();  }
  virtual Bool_t   PxPyPz(Double_t p[3]) const { p[0]=Px(); p[1]=Py(); p[2]=Pz(); return kTRUE; }
  
  virtual Double_t Xv() const { return fPair.GetX(); }
  virtual Double_t Yv() const { return fPair.GetY(); }
  virtual Double_t Zv() const { return fPair.GetZ(); }
  virtual Bool_t   XvYvZv(Double_t x[3]) const { x[0]=Xv(); x[1]=Yv(); x[2]=Zv(); return kTRUE; }
  
  virtual Double_t OneOverPt() const { return Pt()>0.?1./Pt():0.; }  //TODO: check
  virtual Double_t Phi()       const { return fPair.GetPhi();}
  virtual Double_t Theta()     const { return Pz()!=0?TMath::ATan(Pt()/Pz()):0.; } //TODO: check
  
  
  virtual Double_t E() const { return fPair.GetE();    }
  virtual Double_t M() const { return fPair.GetMass(); }
  
  virtual Double_t Eta() const { return fPair.GetEta();}
  virtual Double_t Y()  const  { 
    if((E()*E()-Px()*Px()-Py()*Py()-Pz()*Pz())>0.) return TLorentzVector(Px(),Py(),Pz(),E()).Rapidity();
    else return -1111.;
  }
  
  //  virtual Short_t Charge() const    { return fPair.GetQ();}
  //  void SetProductionVertex(const KFParticle &Vtx) { fPair.SetProductionVertex(Vtx); }

  //inter leg information
  Double_t GetChi2()              const { return fPair.GetChi2();                               }
  Int_t    GetNdf()               const { return fPair.GetNDF();                                }
  Double_t GetDecayLength()       const { return fPair.GetDecayLength();                        }
  Double_t GetR()                 const { return fPair.GetR();                                  }
  Double_t OpeningAngle()         const { return fD1.GetAngle(fD2);                             }
  Double_t OpeningAngleXY()       const { return fD1.GetAngleXY(fD2);                           }
  Double_t OpeningAngleRZ()       const { return fD1.GetAngleRZ(fD2);                           }
  Double_t DistanceDaughters()    const { return fD1.GetDistanceFromParticle(fD2);              }
  Double_t DistanceDaughtersXY()  const { return fD1.GetDistanceFromParticleXY(fD2);            }
  Double_t DeviationDaughters()   const { return fD1.GetDeviationFromParticle(fD2);             }
  Double_t DeviationDaughtersXY() const { return fD1.GetDeviationFromParticleXY(fD2);           }
  Double_t DeltaEta()             const { return TMath::Abs(fD1.GetEta()-fD2.GetEta());         }
  Double_t DeltaPhi()             const { return fD1.GetAngleXY(fD2);                           }
  Double_t DaughtersP()           const { return fD1.GetP()*fD2.GetP();                         }

  // calculate cos(theta*) and phi* in HE and CS pictures
  void GetThetaPhiCM(Double_t &thetaHE, Double_t &phiHE, Double_t &thetaCS, Double_t &phiCS) const;

  Double_t PsiPair(Double_t MagField)const; //Angle cut w.r.t. to magnetic field
  Double_t PhivPair(Double_t MagField)const; //Angle of ee plane w.r.t. to magnetic field

  // TODO: replace by KFParticleBase functions?
  Double_t GetArmAlpha() const;
  Double_t GetArmPt()    const;

  // internal KF particle
  const KFParticle& GetKFParticle()       const { return fPair; }
  const KFParticle& GetKFFirstDaughter()  const { return fD1;   }
  const KFParticle& GetKFSecondDaughter() const { return fD2;   }

  // rotations
  virtual void RotateTrack(PairAnalysisTrackRotator* /*rot*/) { return; }


private:

  KFParticle fPair;   // KF particle internally used for pair calculation
  KFParticle fD1;     // KF particle first daughter
  KFParticle fD2;     // KF particle1 second daughter

  ClassDef(PairAnalysisPairKF,1)
};

#endif

