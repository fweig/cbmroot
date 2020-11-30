///////////////////////////////////////////////////////////////////////////
//
//
// Authors:
//   Julian Book   <Julian.Book@cern.ch>
/*

  PairAnalysisPair class that internally makes use of TLorentzVector.

*/
//                                                                       //
///////////////////////////////////////////////////////////////////////////


#include <TDatabasePDG.h>

#include "CbmL1.h"
#include "L1Algo.h"
#include "L1Field.h"

#include "CbmMCTrack.h"
#include "PairAnalysisTrack.h"

#include "PairAnalysisPairLV.h"

ClassImp(PairAnalysisPairLV)

  PairAnalysisPairLV::PairAnalysisPairLV()
  : PairAnalysisPair(), fPairPos(), fPair(), fD1(), fD2() {
  //
  // Default Constructor
  //
}

//______________________________________________
PairAnalysisPairLV::PairAnalysisPairLV(const PairAnalysisPair& pair)
  : PairAnalysisPair(pair), fPairPos(), fPair(), fD1(), fD2() {
  //
  // Copy Constructor
  //
  SetTracks(pair.GetFirstDaughter(),
            pair.GetFirstDaughterPid(),
            pair.GetSecondDaughter(),
            pair.GetSecondDaughterPid());
}

//______________________________________________
PairAnalysisPairLV::PairAnalysisPairLV(PairAnalysisTrack* const particle1,
                                       Int_t pid1,
                                       PairAnalysisTrack* const particle2,
                                       Int_t pid2,
                                       Char_t type)
  : PairAnalysisPair(type), fPairPos(), fPair(), fD1(), fD2() {
  //
  // Constructor with tracks
  //
  SetTracks(particle1, pid1, particle2, pid2);
}

//______________________________________________
PairAnalysisPairLV::~PairAnalysisPairLV() {
  //
  // Default Destructor
  //
}

//______________________________________________
void PairAnalysisPairLV::SetTracks(PairAnalysisTrack* const particle1,
                                   Int_t pid1,
                                   PairAnalysisTrack* const particle2,
                                   Int_t pid2) {
  //
  // set TLorentzVector daughters and pair
  // refParticle1 and 2 are the original tracks. In the case of track rotation
  // they are needed in the framework
  //

  //vvv
  // TODO: think about moving the pid assignement to PairAnalysisTrack and use it here
  // BUT what about mixed events or LS-pairs
  const Double_t mpid1 = TDatabasePDG::Instance()->GetParticle(pid1)->Mass();
  const Double_t mpid2 = TDatabasePDG::Instance()->GetParticle(pid2)->Mass();
  const Double_t cpid1 =
    TDatabasePDG::Instance()->GetParticle(pid1)->Charge() * 3;
  const Double_t cpid2 =
    TDatabasePDG::Instance()->GetParticle(pid2)->Charge() * 3;

  // match charge of track to pid and set mass accordingly
  fPid1       = pid1;
  fPid2       = pid2;
  Double_t m1 = mpid1;
  Double_t m2 = mpid2;
  if (particle1->Charge() == cpid2) {
    m1    = mpid2;
    fPid1 = pid2;
  }  //TODO: what about 2e-charges
  if (particle2->Charge() == cpid1) {
    m2    = mpid1;
    fPid2 = pid1;
  }

  // Calculate Energy per particle by hand
  Double_t e1 = TMath::Sqrt(m1 * m1 + particle1->Px() * particle1->Px()
                            + particle1->Py() * particle1->Py()
                            + particle1->Pz() * particle1->Pz());

  Double_t e2 = TMath::Sqrt(m2 * m2 + particle2->Px() * particle2->Px()
                            + particle2->Py() * particle2->Py()
                            + particle2->Pz() * particle2->Pz());

  fRefD1 = particle1;
  fRefD2 = particle2;
  fD1.SetPxPyPzE(particle1->Px(), particle1->Py(), particle1->Pz(), e1);
  fD2.SetPxPyPzE(particle2->Px(), particle2->Py(), particle2->Pz(), e2);
  //^^^ this should become obsolete

  // build pair
  fPair    = (fD1 + fD2);
  fPairPos = (*particle1->GetPosition() + *particle2->GetPosition());
  fCharge  = (particle1->Charge() * particle2->Charge());
  fWeight  = TMath::Sqrt(particle1->GetWeight() * particle2->GetWeight());
  //  printf("fill pair weight: %.1f * %.1f = %.1f \n",particle1->GetWeight(),particle2->GetWeight(),fWeight);
}

//______________________________________________
void PairAnalysisPairLV::SetMCTracks(const CbmMCTrack* const particle1,
                                     const CbmMCTrack* const particle2) {
  //
  // build MC pair from TLorentzVector daughters
  // no references are set
  //
  particle1->Get4Momentum(fD1);
  particle2->Get4Momentum(fD2);
  fPair = (fD1 + fD2);
  TLorentzVector fD1Pos(particle1->GetStartX(),
                        particle1->GetStartY(),
                        particle1->GetStartZ(),
                        particle1->GetStartT());
  TLorentzVector fD2Pos(particle2->GetStartX(),
                        particle2->GetStartY(),
                        particle2->GetStartZ(),
                        particle2->GetStartT());
  fPairPos = (fD1Pos + fD2Pos);
}

//______________________________________________
// Int_t PairAnalysisPairLV::Charge() const
// {
//   return (dynamic_cast<PairAnalysisTrack*>(fRefD1.GetObject())->Charge() +
// 	  dynamic_cast<PairAnalysisTrack*>(fRefD2.GetObject())->Charge());
// }

//______________________________________________
void PairAnalysisPairLV::GetThetaPhiCM(Double_t& thetaHE,
                                       Double_t& phiHE,
                                       Double_t& thetaCS,
                                       Double_t& phiCS) const {
  //
  // Calculate theta and phi in helicity and Collins-Soper coordinate frame
  //

  TLorentzVector motherMom(fPair);
  TLorentzVector p1Mom(fD1);
  TLorentzVector p2Mom(fD2);
  PairAnalysisPair::GetThetaPhiCM(
    motherMom, p1Mom, p2Mom, thetaHE, phiHE, thetaCS, phiCS);
}


//______________________________________________
Double_t PairAnalysisPairLV::PsiPair(Double_t /*MagField*/) const {
  //Following idea to use opening of colinear pairs in magnetic field from e.g. PHENIX
  //to ID conversions. Adapted from AliTRDv0Info class
  //TODO: adapt and get magnetic field
  /* (FU) not used
  Double_t x, y;//, z;
  x = fPair.X();
  y = fPair.Y();
  //  z = fPair.Z();
*/
  Double_t m1[3] = {0, 0, 0};
  Double_t m2[3] = {0, 0, 0};

  m1[0] = fD1.Px();
  m1[1] = fD1.Py();
  m1[2] = fD1.Pz();

  m2[0] = fD2.Px();
  m2[1] = fD2.Py();
  m2[2] = fD2.Pz();

  Double_t deltat = 1.;
  //difference of angles of the two daughter tracks with z-axis
  deltat =
    TMath::ATan(m2[2] / (TMath::Sqrt(m2[0] * m2[0] + m2[1] * m2[1]) + 1.e-13))
    - TMath::ATan(m1[2]
                  / (TMath::Sqrt(m1[0] * m1[0] + m1[1] * m1[1]) + 1.e-13));

  //  Double_t radiussum = TMath::Sqrt(x*x + y*y) + 50;//radius to which tracks shall be propagated

  Double_t mom1Prop[3] = {0., 0., 0.};
  Double_t mom2Prop[3] = {0., 0., 0.};

  // TODO: adapt code
  Double_t fPsiPair = 4.;
  /*
  AliExternalTrackParam *d1 = static_cast<AliExternalTrackParam*>(fRefD1.GetObject());
  AliExternalTrackParam *d2 = static_cast<AliExternalTrackParam*>(fRefD2.GetObject());
  AliExternalTrackParam nt(*d1), pt(*d2);

  if(nt.PropagateTo(radiussum,MagField) == 0)//propagate tracks to the outside
	fPsiPair =  -5.;
  if(pt.PropagateTo(radiussum,MagField) == 0)
	fPsiPair = -5.;
  pt.GetPxPyPz(mom1Prop);//Get momentum vectors of tracks after propagation
  nt.GetPxPyPz(mom2Prop);
  */

  // absolute momentum values
  Double_t pEle =
    TMath::Sqrt(mom2Prop[0] * mom2Prop[0] + mom2Prop[1] * mom2Prop[1]
                + mom2Prop[2] * mom2Prop[2]);
  Double_t pPos =
    TMath::Sqrt(mom1Prop[0] * mom1Prop[0] + mom1Prop[1] * mom1Prop[1]
                + mom1Prop[2] * mom1Prop[2]);
  //scalar product of propagated posit
  Double_t scalarproduct = mom1Prop[0] * mom2Prop[0] + mom1Prop[1] * mom2Prop[1]
                           + mom1Prop[2] * mom2Prop[2];
  //Angle between propagated daughter tracks
  Double_t chipair = TMath::ACos(scalarproduct / (pEle * pPos));

  fPsiPair = TMath::Abs(TMath::ASin(deltat / chipair));

  return fPsiPair;
}

//______________________________________________
Double_t PairAnalysisPairLV::GetArmAlpha() const {
  //
  // Calculate the Armenteros-Podolanski Alpha
  //
  Int_t qD1 =
    dynamic_cast<PairAnalysisTrack*>(fRefD1.GetObject())->Charge() > 0;
  TVector3 momNeg = (qD1 < 0 ? fD1.Vect() : fD2.Vect());
  TVector3 momPos = (qD1 < 0 ? fD2.Vect() : fD1.Vect());
  TVector3 momTot(Px(), Py(), Pz());

  Double_t lQlNeg = momNeg.Dot(momTot) / momTot.Mag();
  Double_t lQlPos = momPos.Dot(momTot) / momTot.Mag();

  return ((lQlPos - lQlNeg) / (lQlPos + lQlNeg));
}

//______________________________________________
Double_t PairAnalysisPairLV::GetArmPt() const {
  //
  // Calculate the Armenteros-Podolanski Pt
  //
  Int_t qD1 =
    dynamic_cast<PairAnalysisTrack*>(fRefD1.GetObject())->Charge() > 0;
  TVector3 momNeg = (qD1 < 0 ? fD1.Vect() : fD2.Vect());
  TVector3 momTot(Px(), Py(), Pz());
  return (momNeg.Perp(momTot));
}

//______________________________________________
Double_t PairAnalysisPairLV::PhivPair(Double_t MagField) const {
  /// Following the idea to use opening of collinear pairs in magnetic field from e.g. PHENIX
  /// to identify conversions. Angle between ee plane and magnetic field is calculated (0 to pi).
  /// Due to tracking to the primary vertex, conversions with no intrinsic opening angle
  /// always end up as pair in "cowboy" configuration. The function as defined here then
  /// returns values close to pi.
  /// Correlated Like Sign pairs (from double conversion / dalitz + conversion) may show up
  /// at pi or at 0 depending on which leg has the higher momentum. (not checked yet)
  /// This expected ambiguity is not seen due to sorting of track arrays in this framework.
  /// To reach the same result as for ULS (~pi), the legs are flipped for LS.
  /// TODO: VALIDATE OBSERVABLE
  Int_t qD1 = 0;
  if (fRefD1.GetObject())
    qD1 = dynamic_cast<PairAnalysisTrack*>(fRefD1.GetObject())->Charge() > 0;
  // TODO add mc charge (no fRefD1.GetObject())
  TVector3 p1;
  TVector3 p2;

  //  L1FieldValue bfield = CbmL1::Instance()->algo->GetvtxFieldValue();
  //  printf("l1 field values: %f %f %f \n",bfield.x[0],bfield.y[0],bfield.z[0]);
  //  if(bfield.y[0]>0){
  if (MagField < 0) {
    p1 = (qD1 > 0 ? fD1.Vect() : fD2.Vect());
    p2 = (qD1 > 0 ? fD2.Vect() : fD1.Vect());
  } else {
    p2 = (qD1 > 0 ? fD1.Vect() : fD2.Vect());
    p1 = (qD1 > 0 ? fD2.Vect() : fD1.Vect());
  }

  //unit vector of (pep+pem)
  TVector3 u = fPair.Vect();
  u.SetMag(1.);  // normalize

  //vector product of pep X pem (perpendicular to the pair)
  TVector3 vpm = p1.Cross(p2);
  vpm.SetMag(1.);  // normalize

  //The third axis defined by vector product (ux,uy,uz)X(vx,vy,vz)
  // by construction, (wx,wy,wz) must be a unit vector.
  TVector3 w = u.Cross(vpm);

  // unit vector in xz-plane (perpendicular to B-field)
  Double_t ax =
    u.Pz() / TMath::Sqrt(u.Px() + u.Px() + u.Pz() + u.Pz());  // =sin(alpha_xz)
  Double_t ay = 0.;                                           // by defintion
  Double_t az =
    u.Pz()
    / TMath::Sqrt(u.Px() + u.Px() + u.Pz() + u.Pz());  // =cos(alpha_xz+180)
  TVector3 a(ax, ay, az);

  // measure angle between (wx,wy,wz) and (ax,ay,0). The angle between them
  // should be small if the pair is conversion
  // Double_t cosPhiV = w.Px()*ax + w.Py()*ay; // angle btw w and a
  // Double_t phiv = TMath::ACos(cosPhiV);
  Double_t phiv = w.Angle(a);

  return phiv;
}

//_______________________________________________
void PairAnalysisPairLV::RotateTrack(PairAnalysisTrackRotator* rot) {
  //
  // Rotate one of the legs according to the track rotator settings
  //

  Double_t rotAngle  = rot->GetAngle();
  Double_t rotCharge = rot->GetCharge();

  PairAnalysisTrack* first = GetFirstDaughter();
  if (!first) return;

  //Printf("\n before rotation:");
  //fD1.Print("");
  //fD2.Print("");

  if (rot->GetRotationType()
        == PairAnalysisTrackRotator::ERotationType::kRotatePositive
      || (rot->GetRotationType()
            == PairAnalysisTrackRotator::ERotationType::kRotateBothRandom
          && rotCharge == 0)) {
    if (first->Charge() > 0)
      fD1.RotateZ(rotAngle);
    else
      fD2.RotateZ(rotAngle);
  }

  if (rot->GetRotationType()
        == PairAnalysisTrackRotator::ERotationType::kRotateNegative
      || (rot->GetRotationType()
            == PairAnalysisTrackRotator::ERotationType::kRotateBothRandom
          && rotCharge == 1)) {
    if (first->Charge() > 0)
      fD1.RotateZ(rotAngle);
    else
      fD2.RotateZ(rotAngle);
  }
  //  Printf("after rotation:");
  //fD1.Print("");
  //fD2.Print("");

  // rebuild pair
  fPair = (fD1 + fD2);
}
