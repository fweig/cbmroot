#ifndef PAIRANALYSISPAIRLV_H
#define PAIRANALYSISPAIRLV_H

//#############################################################
//#                                                           #
//#                  PairAnalysisPairLV                         #
//#               Class to store pair information             #
//#                                                           #
//#                                                           #
//#  Authors:                                                 #
//#   Julian    Book,     Uni Ffm / Julian.Book@cern.ch       #
//#                                                           #
//#############################################################

#include <TMath.h>
#include <TLorentzVector.h>

#include "PairAnalysisTrack.h"
#include "PairAnalysisTrackRotator.h"
#include "PairAnalysisPair.h"

class CbmVertex;
class CbmMCTrack;

class PairAnalysisPairLV : public PairAnalysisPair {
public:
  PairAnalysisPairLV();
  virtual ~PairAnalysisPairLV();
  PairAnalysisPairLV(const PairAnalysisPair& pair);

  PairAnalysisPairLV(PairAnalysisTrack * const particle1, Int_t pid1,
		     PairAnalysisTrack * const particle2, Int_t pid2, Char_t type);

  void SetTracks(PairAnalysisTrack * const particle1, Int_t pid1,
                 PairAnalysisTrack * const particle2, Int_t pid2);

  void SetMCTracks(const CbmMCTrack * const particle1,
		   const CbmMCTrack * const particle2);


  // kinematics
  virtual Double_t Px() const { return fPair.Px(); }
  virtual Double_t Py() const { return fPair.Py(); }
  virtual Double_t Pz() const { return fPair.Pz(); }
  virtual Double_t Pt() const { return fPair.Pt(); }
  virtual Double_t P() const  { return fPair.P();  }
  virtual Bool_t   PxPyPz(Double_t p[3]) const { p[0]=Px(); p[1]=Py(); p[2]=Pz(); return kTRUE; }
  
  virtual Double_t Xv() const { return fPairPos.X(); }
  virtual Double_t Yv() const { return fPairPos.Y(); }
  virtual Double_t Zv() const { return fPairPos.Z(); }
  virtual Bool_t   XvYvZv(Double_t x[3]) const { x[0]=Xv(); x[1]=Yv(); x[2]=Zv(); return kTRUE; }
  
  virtual Double_t OneOverPt() const { return Pt()>0.?1./Pt():0.; }  //TODO: check
  virtual Double_t Phi()       const { return fPair.Phi();}
  virtual Double_t Theta()     const { return Pz()!=0?TMath::ATan(Pt()/Pz()):0.; } //TODO: check
  
  
  virtual Double_t E() const { return fPair.E();    }
  virtual Double_t M() const { return fPair.M(); }
  
  //  virtual Double_t Eta() const { return fPair.Eta();}
  virtual Double_t Y()  const  { return fPair.Rapidity(); }
  
  //  virtual Int_t Charge() const {return fCharge; }

  //inter leg information
  Double_t GetR()                 const { return fPairPos.Vect().Perp();                   }
  Double_t OpeningAngle()         const { return fD1.Angle(fD2.Vect());                    }
  Double_t DeltaEta()             const { return TMath::Abs(fD1.Eta()-fD2.Eta());          }
  Double_t DeltaPhi()             const { return fD1.DeltaPhi(fD2);                        }
  Double_t DaughtersP()           const { return fD1.P()*fD2.P();                          }

  // calculate cos(theta*) and phi* in HE and CS pictures
  void GetThetaPhiCM(Double_t &thetaHE, Double_t &phiHE, Double_t &thetaCS, Double_t &phiCS) const;

  Double_t PsiPair(Double_t MagField)const; //Angle cut w.r.t. to magnetic field
  Double_t PhivPair(Double_t MagField)const; //Angle of ee plane w.r.t. to magnetic field

  Double_t GetArmAlpha() const;
  Double_t GetArmPt()    const;

  // internal Lorentz vector particle
  const TLorentzVector& GetLVParticle()       const { return fPair; }
  const TLorentzVector& GetLVFirstDaughter()  const { return fD1;   }
  const TLorentzVector& GetLVSecondDaughter() const { return fD2;   }

  // rotations
  virtual void RotateTrack(PairAnalysisTrackRotator *rot);

private:

  TLorentzVector fPairPos;// lorentz vector position
  TLorentzVector fPair;   // lorentz vector internaly used for pair calculation
  TLorentzVector fD1;     // lorentz vector first daughter
  TLorentzVector fD2;     // lorentz vector second daughter

  ClassDef(PairAnalysisPairLV,2) // Lorentz vector pairs
};

#endif

