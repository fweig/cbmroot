///////////////////////////////////////////////////////////////////////////
//
//
// Authors:
//   Julian Book   <Julian.Book@cern.ch>
/*

  PairAnalysis pair interface class.

*/
//                                                                       //
///////////////////////////////////////////////////////////////////////////


#include <TDatabasePDG.h>

#include "CbmVertex.h"

#include "PairAnalysisTrack.h"
#include "PairAnalysisPair.h"

ClassImp(PairAnalysisPair)

Double_t PairAnalysisPair::fBeamEnergy=-1.;

PairAnalysisPair::PairAnalysisPair() :
  PairAnalysisTrack(),
  fRefD1(),
  fRefD2()
{
  //
  // Default Constructor
  //

}

//______________________________________________
PairAnalysisPair::PairAnalysisPair(const PairAnalysisPair& pair) :
  PairAnalysisTrack(pair),
  fType(pair.GetType()),
  fCharge(pair.Charge()),
  fLabel(pair.GetLabel()),
  fWeight(pair.GetWeight()),
  fPdgCode(pair.PdgCode()),
  fRefD1(pair.GetFirstDaughter()),
  fRefD2(pair.GetSecondDaughter()),
  fPid1(pair.GetFirstDaughterPid()),
  fPid2(pair.GetSecondDaughterPid()),
  fKFUsage(pair.GetKFUsage())
{
  //
  // Copy Constructor
  //

}

//______________________________________________
PairAnalysisPair::PairAnalysisPair(Char_t type) :
  fType(type),
  fRefD1(),
  fRefD2()
{
  //
  // Constructor with type
  //

}

//______________________________________________
PairAnalysisPair::~PairAnalysisPair()
{
  //
  // Default Destructor
  //

}

//______________________________________________
void PairAnalysisPair::GetThetaPhiCM(TLorentzVector &motherMom, TLorentzVector &p1Mom, TLorentzVector &p2Mom,
				     Double_t &thetaHE, Double_t &phiHE, Double_t &thetaCS, Double_t &phiCS) const
{
  //
  // Calculate theta and phi in helicity and Collins-Soper coordinate frame
  //

  const Double_t proMass = TDatabasePDG::Instance()->GetParticle(2212)->Mass();
  TLorentzVector projMom(0.,0., fBeamEnergy,TMath::Sqrt(fBeamEnergy*fBeamEnergy+proMass*proMass));
  TLorentzVector targMom(0.,0., 0.,         TMath::Sqrt(0.*0+proMass*proMass));

  // boost all the 4-mom vectors to the mother rest frame
  TVector3 beta = (-1.0/motherMom.E())*motherMom.Vect();
  p1Mom.Boost(beta);
  p2Mom.Boost(beta);
  projMom.Boost(beta);
  targMom.Boost(beta);

  // x,y,z axes
  TVector3 zAxisHE = (motherMom.Vect()).Unit();
  TVector3 zAxisCS = ((projMom.Vect()).Unit()-(targMom.Vect()).Unit()).Unit();
  TVector3 yAxis = ((projMom.Vect()).Cross(targMom.Vect())).Unit();
  TVector3 xAxisHE = (yAxis.Cross(zAxisHE)).Unit();
  TVector3 xAxisCS = (yAxis.Cross(zAxisCS)).Unit();

  // fill theta and phi
  if(static_cast<PairAnalysisTrack*>(fRefD1.GetObject())->Charge()>0){
    thetaHE = zAxisHE.Dot((p1Mom.Vect()).Unit());
    thetaCS = zAxisCS.Dot((p1Mom.Vect()).Unit());
    phiHE   = TMath::ATan2((p1Mom.Vect()).Dot(yAxis), (p1Mom.Vect()).Dot(xAxisHE));
    phiCS   = TMath::ATan2((p1Mom.Vect()).Dot(yAxis), (p1Mom.Vect()).Dot(xAxisCS));
  } else {
    thetaHE = zAxisHE.Dot((p2Mom.Vect()).Unit());
    thetaCS = zAxisCS.Dot((p2Mom.Vect()).Unit());
    phiHE   = TMath::ATan2((p2Mom.Vect()).Dot(yAxis), (p2Mom.Vect()).Dot(xAxisHE));
    phiCS   = TMath::ATan2((p2Mom.Vect()).Dot(yAxis), (p2Mom.Vect()).Dot(xAxisCS));
  }

}

//______________________________________________
Double_t PairAnalysisPair::GetCosPointingAngle(const CbmVertex *primVtx) const
{
  //
  // Calculate the poiting angle of the pair to the primary vertex and take the cosine
  //
  if(!primVtx) return -1.;

  TVector3 pairPos(Xv(),Yv(),Zv());
  TVector3 pvtxPos;
  primVtx->Position(pvtxPos);
  pairPos-=pvtxPos;   //vector between the reference point and the V0 vertex

  TVector3 pairMom(Px(),Py(),Pz());
  Double_t pointingAngle = pairMom.Angle(pairPos);
  return TMath::Abs( TMath::Cos(pointingAngle) );

}

//______________________________________________
void PairAnalysisPair::GetDCA(const CbmVertex *primVtx, Double_t d0z0[2]) const
{
  //
  // Calculate the dca of the mother with respect to the primary vertex
  //
  if(!primVtx) return;

  d0z0[0] = TMath::Sqrt(TMath::Power(Xv()-primVtx->GetX(),2) +
			TMath::Power(Yv()-primVtx->GetY(),2) );

  d0z0[1] = Zv() - primVtx->GetZ();
  return;

}

//______________________________________________
void PairAnalysisPair::SetBeamEnergy(Double_t beamEbyHand)
{
  //
  // set the beam energy by hand
  //
  fBeamEnergy = beamEbyHand;
}


