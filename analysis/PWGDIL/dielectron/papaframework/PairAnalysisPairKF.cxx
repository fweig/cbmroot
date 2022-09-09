/*************************************************************************
* Copyright(c) 1998-2009, ALICE Experiment at CERN, All rights reserved. *
**************************************************************************/

///////////////////////////////////////////////////////////////////////////
//                                                                       //
//  PairAnalysis Pair class. Internally it makes use of KFParticle.     //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

#include "PairAnalysisPairKF.h"

#include "CbmKFParticleInterface.h"
#include "CbmKFTrack.h"
#include "CbmMCTrack.h"
#include "CbmVertex.h"

#include <TDatabasePDG.h>

#include "KFParticle.h"
#include "PairAnalysisTrack.h"

ClassImp(PairAnalysisPairKF)

  PairAnalysisPairKF::PairAnalysisPairKF()
  : PairAnalysisPair()
  , fPair()
  , fD1()
  , fD2()
{
  //
  // Default Constructor
  //
}

//______________________________________________
PairAnalysisPairKF::PairAnalysisPairKF(const PairAnalysisPair& pair) : PairAnalysisPair(pair), fPair(), fD1(), fD2()
{
  //
  // Copy Constructor
  //
  SetTracks(pair.GetFirstDaughter(), pair.GetFirstDaughterPid(), pair.GetSecondDaughter(), pair.GetSecondDaughterPid());
}

//______________________________________________
PairAnalysisPairKF::PairAnalysisPairKF(PairAnalysisTrack* const particle1, Int_t pid1,
                                       PairAnalysisTrack* const particle2, Int_t pid2, Char_t type)
  : PairAnalysisPair(type)
  , fPair()
  , fD1()
  , fD2()
{
  //
  // Constructor with tracks
  //
  SetTracks(particle1, pid1, particle2, pid2);
}

//______________________________________________
PairAnalysisPairKF::~PairAnalysisPairKF()
{
  //
  // Default Destructor
  //
}

//______________________________________________
void PairAnalysisPairKF::SetTracks(PairAnalysisTrack* const particle1, Int_t pid1, PairAnalysisTrack* const particle2,
                                   Int_t pid2)
{
  //
  // set KF daughters and pair
  // refParticle1 and 2 are the original tracks. In the case of track rotation
  // they are needed in the framework
  //
  // TODO: think about moving the pid assignement to PairAnalysisTrack and use it here
  // BUT think about mixed events or LS-pairs
  //  const Double_t mpid1 = TDatabasePDG::Instance()->GetParticle(pid1)->Mass(); (FU) unused
  //  const Double_t mpid2 = TDatabasePDG::Instance()->GetParticle(pid2)->Mass(); (FU) unused
  const Double_t cpid1 = TDatabasePDG::Instance()->GetParticle(pid1)->Charge() * 3;
  const Double_t cpid2 = TDatabasePDG::Instance()->GetParticle(pid2)->Charge() * 3;

  // match charge of track to pid and set mass accordingly
  fPid1 = pid1;
  fPid2 = pid2;
  //  Double_t m1 = mpid1; (FU) unused
  //  Double_t m2 = mpid2; (FU) unused
  if (particle1->Charge() == cpid2) {
    //    m1=mpid2*; (FU) unused
    fPid1 = pid2;
  }  //TODO: what about 2e-charges
  if (particle2->Charge() == cpid1) {
    //    m2=mpid1; (FU) unused
    fPid2 = pid1;
  }

  /// this interface refits the sts track according to the pdg code, fails when refit is done
  /// be carefull in Mixed events this does not work because STS hits are not there
  //// TODO: - write converter w/o refit, what about field coefficients
  ////       - OR store the KFparticle in the PapaTrack and use it here instead
  CbmKFParticleInterface::SetKFParticleFromStsTrack(particle1->GetStsTrack(), &fD1, fPid1, kTRUE);
  CbmKFParticleInterface::SetKFParticleFromStsTrack(particle2->GetStsTrack(), &fD2, fPid2, kTRUE);

  // references
  fRefD1 = particle1;
  fRefD2 = particle2;

  // build pair
  fPair.Initialize();

  fPair.AddDaughter(fD1);
  fPair.AddDaughter(fD2);

  /// mass constrain
  // Double_t mass = TDatabasePDG::Instance()->GetParticle(443)->Mass();
  // Double_t wdth = TDatabasePDG::Instance()->GetParticle(443)->Width();
  // if(wdth<1.e-6) wdth=mass*0.01; // width<1keV, then 1% mass resolution
  //  fPair.SetMassConstraint( mass, wdth ); //TODO: take from mother pdg code provided to pairanalysis

  fCharge = (particle1->Charge() * particle2->Charge());
  fWeight = TMath::Sqrt(particle1->GetWeight() * particle2->GetWeight());
  //  printf("fill pair weight: %.1f * %.1f = %.1f \n",particle1->GetWeight(),particle2->GetWeight(),fWeight);
}

//______________________________________________
void PairAnalysisPairKF::SetMCTracks(const CbmMCTrack* const particle1, const CbmMCTrack* const particle2)
{
  //
  // build MC pair from daughters
  // no references are set
  //

  //Initialise covariance matrix and set current parameters to 0.0
  KFParticle kf1;
  kf1.Initialize();
  Float_t* par1 = kf1.Parameters();
  par1[0]       = particle1->GetStartX();
  par1[1]       = particle1->GetStartY();
  par1[2]       = particle1->GetStartZ();
  par1[3]       = particle1->GetPx();
  par1[4]       = particle1->GetPy();
  par1[5]       = particle1->GetPz();
  par1[6]       = particle1->GetEnergy();
  kf1.SetPDG(particle1->GetPdgCode());

  KFParticle kf2;
  kf2.Initialize();
  Float_t* par2 = kf2.Parameters();
  par2[0]       = particle2->GetStartX();
  par2[1]       = particle2->GetStartY();
  par2[2]       = particle2->GetStartZ();
  par2[3]       = particle2->GetPx();
  par2[4]       = particle2->GetPy();
  par2[5]       = particle2->GetPz();
  par2[6]       = particle2->GetEnergy();
  kf2.SetPDG(particle2->GetPdgCode());

  // build pair
  fPair.Initialize();
  fPair.AddDaughter(kf1);
  fPair.AddDaughter(kf2);
}

//______________________________________________
void PairAnalysisPairKF::GetThetaPhiCM(Double_t& thetaHE, Double_t& phiHE, Double_t& thetaCS, Double_t& phiCS) const
{
  //
  // Calculate theta and phi in helicity and Collins-Soper coordinate frame
  //
  Double_t px1          = fD1.GetPx();
  Double_t py1          = fD1.GetPy();
  Double_t pz1          = fD1.GetPz();
  Double_t px2          = fD2.GetPx();
  Double_t py2          = fD2.GetPy();
  Double_t pz2          = fD2.GetPz();
  const Double_t d1Mass = TDatabasePDG::Instance()->GetParticle(fPid1)->Mass();
  const Double_t d2Mass = TDatabasePDG::Instance()->GetParticle(fPid2)->Mass();

  // first & second daughter 4-mom
  TLorentzVector p1Mom(px1, py1, pz1, TMath::Sqrt(px1 * px1 + py1 * py1 + pz1 * pz1 + d1Mass * d1Mass));
  TLorentzVector p2Mom(px2, py2, pz2, TMath::Sqrt(px2 * px2 + py2 * py2 + pz2 * pz2 + d2Mass * d2Mass));
  // mother 4-momentum vector
  TLorentzVector motherMom = p1Mom + p2Mom;

  PairAnalysisPair::GetThetaPhiCM(motherMom, p1Mom, p2Mom, thetaHE, phiHE, thetaCS, phiCS);
}


//______________________________________________
Double_t PairAnalysisPairKF::PsiPair(Double_t /*MagField*/) const
{
  return 0.; /*
  //Following idea to use opening of colinear pairs in magnetic field from e.g. PHENIX
  //to ID conversions. Adapted from TRDv0Info class
  Double_t x, y;//, z;
  x = fPair.GetX();
  y = fPair.GetY();
  //  z = fPair.GetZ();

  Double_t m1[3] = {0,0,0};
  Double_t m2[3] = {0,0,0};

  m1[0] = fD1.GetPx();
  m1[1] = fD1.GetPy();
  m1[2] = fD1.GetPz();  

  m2[0] = fD2.GetPx();
  m2[1] = fD2.GetPy();
  m2[2] = fD2.GetPz();

  Double_t deltat = 1.;
  deltat = TMath::ATan(m2[2]/(TMath::Sqrt(m2[0]*m2[0] + m2[1]*m2[1])+1.e-13))-
	TMath::ATan(m1[2]/(TMath::Sqrt(m1[0]*m1[0] + m1[1]*m1[1])+1.e-13));//difference of angles of the two daughter tracks with z-axis

  Double_t radiussum = TMath::Sqrt(x*x + y*y) + 50;//radius to which tracks shall be propagated

  Double_t mom1Prop[3];
  Double_t mom2Prop[3];

  ExternalTrackParam *d1 = static_cast<ExternalTrackParam*>(fRefD1.GetObject());
  ExternalTrackParam *d2 = static_cast<ExternalTrackParam*>(fRefD2.GetObject());

  ExternalTrackParam nt(*d1), pt(*d2);

  Double_t fPsiPair = 4.;
  if(nt.PropagateTo(radiussum,MagField) == 0)//propagate tracks to the outside
	fPsiPair =  -5.;
  if(pt.PropagateTo(radiussum,MagField) == 0)
	fPsiPair = -5.;
  pt.GetPxPyPz(mom1Prop);//Get momentum vectors of tracks after propagation
  nt.GetPxPyPz(mom2Prop);



  Double_t pEle =
	TMath::Sqrt(mom2Prop[0]*mom2Prop[0]+mom2Prop[1]*mom2Prop[1]+mom2Prop[2]*mom2Prop[2]);//absolute momentum val
  Double_t pPos =
	TMath::Sqrt(mom1Prop[0]*mom1Prop[0]+mom1Prop[1]*mom1Prop[1]+mom1Prop[2]*mom1Prop[2]);//absolute momentum val

  Double_t scalarproduct =
	mom1Prop[0]*mom2Prop[0]+mom1Prop[1]*mom2Prop[1]+mom1Prop[2]*mom2Prop[2];//scalar product of propagated posit

  Double_t chipair = TMath::ACos(scalarproduct/(pEle*pPos));//Angle between propagated daughter tracks

  fPsiPair =  TMath::Abs(TMath::ASin(deltat/chipair));

  return fPsiPair;
	    */
}

//______________________________________________
Double_t PairAnalysisPairKF::GetArmAlpha() const
{
  //
  // Calculate the Armenteros-Podolanski Alpha
  //
  Int_t qD1 = fD1.GetQ();

  TVector3 momNeg((qD1 < 0 ? fD1.GetPx() : fD2.GetPx()), (qD1 < 0 ? fD1.GetPy() : fD2.GetPy()),
                  (qD1 < 0 ? fD1.GetPz() : fD2.GetPz()));
  TVector3 momPos((qD1 < 0 ? fD2.GetPx() : fD1.GetPx()), (qD1 < 0 ? fD2.GetPy() : fD1.GetPy()),
                  (qD1 < 0 ? fD2.GetPz() : fD1.GetPz()));
  TVector3 momTot(Px(), Py(), Pz());

  Double_t lQlNeg = momNeg.Dot(momTot) / momTot.Mag();
  Double_t lQlPos = momPos.Dot(momTot) / momTot.Mag();

  return ((lQlPos - lQlNeg) / (lQlPos + lQlNeg));
}

//______________________________________________
Double_t PairAnalysisPairKF::GetArmPt() const
{
  //
  // Calculate the Armenteros-Podolanski Pt
  //
  Int_t qD1 = fD1.GetQ();

  TVector3 momNeg((qD1 < 0 ? fD1.GetPx() : fD2.GetPx()), (qD1 < 0 ? fD1.GetPy() : fD2.GetPy()),
                  (qD1 < 0 ? fD1.GetPz() : fD2.GetPz()));
  TVector3 momTot(Px(), Py(), Pz());

  return (momNeg.Perp(momTot));
}

//______________________________________________
Double_t PairAnalysisPairKF::PhivPair(Double_t MagField) const
{
  //Following idea to use opening of colinear pairs in magnetic field from e.g. PHENIX
  //to ID conversions. Angle between ee plane and magnetic field is calculated.

  //Define local buffer variables for leg properties
  Double_t px1 = -9999., py1 = -9999., pz1 = -9999.;
  Double_t px2 = -9999., py2 = -9999., pz2 = -9999.;

  if (MagField > 0) {
    if (fD1.GetQ() > 0) {
      px1 = fD1.GetPx();
      py1 = fD1.GetPy();
      pz1 = fD1.GetPz();

      px2 = fD2.GetPx();
      py2 = fD2.GetPy();
      pz2 = fD2.GetPz();
    }
    else {
      px1 = fD2.GetPx();
      py1 = fD2.GetPy();
      pz1 = fD2.GetPz();

      px2 = fD1.GetPx();
      py2 = fD1.GetPy();
      pz2 = fD1.GetPz();
    }
  }
  else {
    if (fD1.GetQ() > 0) {
      px1 = fD2.GetPx();
      py1 = fD2.GetPy();
      pz1 = fD2.GetPz();

      px2 = fD1.GetPx();
      py2 = fD1.GetPy();
      pz2 = fD1.GetPz();
    }
    else {
      px1 = fD1.GetPx();
      py1 = fD1.GetPy();
      pz1 = fD1.GetPz();

      px2 = fD2.GetPx();
      py2 = fD2.GetPy();
      pz2 = fD2.GetPz();
    }
  }

  Double_t px     = px1 + px2;
  Double_t py     = py1 + py2;
  Double_t pz     = pz1 + pz2;
  Double_t dppair = TMath::Sqrt(px * px + py * py + pz * pz);

  //unit vector of (pep+pem)
  Double_t pl = dppair;
  Double_t ux = px / pl;
  Double_t uy = py / pl;
  Double_t uz = pz / pl;
  Double_t ax = uy / TMath::Sqrt(ux * ux + uy * uy);
  Double_t ay = -ux / TMath::Sqrt(ux * ux + uy * uy);

  //momentum of e+ and e- in (ax,ay,az) axis. Note that az=0 by
  //definition.
  //Double_t ptep = iep->Px()*ax + iep->Py()*ay;
  //Double_t ptem = iem->Px()*ax + iem->Py()*ay;

  Double_t pxep = px1;
  Double_t pyep = py1;
  Double_t pzep = pz1;
  Double_t pxem = px2;
  Double_t pyem = py2;
  Double_t pzem = pz2;

  //vector product of pep X pem
  Double_t vpx = pyep * pzem - pzep * pyem;
  Double_t vpy = pzep * pxem - pxep * pzem;
  Double_t vpz = pxep * pyem - pyep * pxem;
  Double_t vp  = sqrt(vpx * vpx + vpy * vpy + vpz * vpz);
  //Double_t thev = acos(vpz/vp);

  //unit vector of pep X pem
  Double_t vx = vpx / vp;
  Double_t vy = vpy / vp;
  Double_t vz = vpz / vp;

  //The third axis defined by vector product (ux,uy,uz)X(vx,vy,vz)
  Double_t wx = uy * vz - uz * vy;
  Double_t wy = uz * vx - ux * vz;
  //Double_t wz = ux*vy - uy*vx;
  //Double_t wl = sqrt(wx*wx+wy*wy+wz*wz);
  // by construction, (wx,wy,wz) must be a unit vector.
  // measure angle between (wx,wy,wz) and (ax,ay,0). The angle between them
  // should be small if the pair is conversion
  //
  Double_t cosPhiV = wx * ax + wy * ay;
  Double_t phiv    = TMath::ACos(cosPhiV);

  return phiv;
}
