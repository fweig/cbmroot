/** @file CbmTofTrackletTools.cxx
 ** @author nh
 ** @date 28.02.2020
 **
 **/

#include "CbmTofTrackletTools.h"
#include "CbmTofTracklet.h"
#include "CbmTofHit.h"
#include "LKFMinuit.h"

#include "FairLogger.h"

#include "Rtypes.h"                        // for Double_t, Double32_t, Int_t, etc
#include "TMatrixFSymfwd.h"         // for TMatrixFSym
#include "TMatrixD.h"
#include "TVectorD.h"
#include "TDecompSVD.h"

using std::vector;
LKFMinuit fMinuit;

CbmTofTrackletTools::CbmTofTrackletTools()
{
  fMinuit=*LKFMinuit::Instance();
  //if( &fMinuit == NULL ) fMinuit.Initialize();
}

CbmTofTrackletTools::~CbmTofTrackletTools() {
}

Double_t* CbmTofTrackletTools::Line3DFit( CbmTofTracklet *pTrk, Int_t iDetId ){
  TGraph2DErrors * gr = new TGraph2DErrors();
  Int_t NHit=0;
  for (Int_t iHit=0; iHit<pTrk->GetNofHits(); iHit++){
    if ( iDetId == pTrk->GetTofDetIndex(iHit) ) continue; // skip this hit of tracklet
    gr->SetPoint(NHit,pTrk->GetTofHitPointer(iHit)->GetX(),pTrk->GetTofHitPointer(iHit)->GetY(),pTrk->GetTofHitPointer(iHit)->GetZ());
    gr->SetPointError(NHit,pTrk->GetTofHitPointer(iHit)->GetDx(),pTrk->GetTofHitPointer(iHit)->GetDy(),pTrk->GetTofHitPointer(iHit)->GetDz());
    NHit++;
  }
   // fit the graph now 
   Double_t pStart[4]= {0.,0.,0.,0.};
   pStart[0]=pTrk->GetFitX(0.);
   pStart[1]=(pTrk->GetTrackParameter())->GetTx();
   pStart[2]=pTrk->GetFitY(0.);
   pStart[3]=(pTrk->GetTrackParameter())->GetTy();
   LOG(debug) << "Line3DFit init: X0 "
	      <<pStart[0]<<", TX "<<pStart[1]<<", Y0 "<<pStart[2]<<", TY "<<pStart[3];
   fMinuit.DoFit(gr,pStart);
   gr->Delete();
   Double_t* dRes;
   //   LOG(info) << "Get fit results ";
   dRes=fMinuit.GetParFit();
   LOG(debug) << "Line3Dfit result: "
	      <<gMinuit->fCstatu<<" : X0 "
	      <<dRes[0]<<", TX "<<dRes[1]<<", Y0 "<<dRes[2]<<", TY "<<dRes[3]
	      << ", Chi2DoF: "<< fMinuit.GetChi2DoF();
   return dRes;
}

Double_t CbmTofTrackletTools::FitTt( CbmTofTracklet *pTrk, Int_t iDetId ){
  Int_t    nValidHits=0;
  Int_t    iHit1=0;
  Double_t dDist1=100.;
  //LOG(info) << "FitTt: " << pTrk->GetNofHits() << " hits, exclude " << Form("0x%08x",iDetId);  
  Double_t aR[pTrk->GetNofHits()-1];
  Double_t at[pTrk->GetNofHits()-1];
  Double_t ae[pTrk->GetNofHits()-1];
  for (Int_t iHit=0; iHit<pTrk->GetNofHits(); iHit++){
    if ( iDetId == pTrk->GetTofDetIndex(iHit) ) continue;
    if(nValidHits==0){
      iHit1=iHit;
      //LOG(info) << "Init Dist1 with " << iHit1;
      Double_t *dPar=Line3DFit(pTrk, iDetId); // spatial fit without iDetId 
      dDist1=pTrk->GetTofHitPointer(iHit1)->GetZ()*TMath::Sqrt(1.+dPar[1]*dPar[1]+dPar[3]*dPar[3]);
      //LOG(info) << "Dist1 = " << dDist1;
    }
    Double_t dSign=1.;
    if(pTrk->GetTofHitPointer(iHit)->GetZ() < pTrk->GetTofHitPointer(iHit1)->GetZ()) dSign=-1.; 
    aR[nValidHits]=dDist1+dSign*pTrk->Dist3D(pTrk->GetTofHitPointer(iHit),pTrk->GetTofHitPointer(iHit1));
    at[nValidHits]=pTrk->GetTofHitPointer(iHit)->GetTime();
    ae[nValidHits]=0.1;                         // const timing error, FIXME (?)
    //LOG(info) << "Init vector index " << nValidHits;
    nValidHits++;    
  }
  if( nValidHits  < 2 ) return 0.;
  if( nValidHits == 2 ) return (at[0]-at[1])/(aR[0]-aR[1]);
  
  Double_t RRsum=0;            //  Values will follow this procedure:
  Double_t Rsum=0;             //  $Rsum=\sum_{i}^{nValidHits}\frac{R_i}{e_i^2}$
  Double_t tsum=0;             //  where e_i will always be the error on the t measurement 
  Double_t esum=0;             //  RR=R^2 in numerator, e denotes 1 in numerator , Rt= R*t in numerator
  Double_t Rtsum=0;            //
  Double_t sig_weight=0;       //  ae^2
  Double_t yoffset=at[0]-10;      //  T0 time offset to scale times to ns regime and not 10^10ns
  for (Int_t i=0; i<nValidHits;i++) {
    at[i]-=yoffset;            //  substract offset
    sig_weight=(ae[i]*ae[i]);
    Rsum+=(aR[i]/sig_weight);
    tsum+=(at[i]/sig_weight);
    RRsum+=(aR[i]*aR[i]/sig_weight);
    Rtsum+=(aR[i]*at[i]/sig_weight);
    esum+=(1/sig_weight);
  }
  Double_t det_cov_mat=esum*RRsum-Rsum*Rsum;  // Determinant of inverted Covariance Matrix -> 1/det is common Faktor of Cavariance Matrix
  //fT0=(RRsum*tsum-Rsum*Rtsum)/det_cov_mat;    // Best Guess for time at origin
  return (-Rsum*tsum+esum*Rtsum)/det_cov_mat;    // Best guess for inverted velocity
  //fT0Err=TMath::Sqrt(RRsum/det_cov_mat);      // sqrt of (0,0) in Covariance matrix -> error on fT0
  //fTtErr=TMath::Sqrt(esum/det_cov_mat);       // sqrt of (1,1) in Covariance Matrix -> error on fTt
  //fT0TtCov=-Rsum/det_cov_mat;                 // (0,1)=(1,0) in Covariance Matrix -> cov(fT0,fTt)

  //fT0+=yoffset;                               // Adding yoffset again  
}

Double_t CbmTofTrackletTools::GetXdif(CbmTofTracklet *pTrk, Int_t iDetId, CbmTofHit* pHit) {
  Double_t dXref = 0.;
  Int_t iNref = 0;
  Double_t dTx=0;
  
  if(1) {
    for (Int_t iHL=0; iHL<pTrk->GetNofHits()-1; iHL++) {
     if (iDetId == pTrk->GetTofDetIndex(iHL) || 0 == pTrk->GetTofDetIndex(iHL)) continue;           // exclude faked hits 
     for (Int_t iHH=iHL+1; iHH<pTrk->GetNofHits(); iHH++) {
       if (iDetId == pTrk->GetTofDetIndex(iHH) || 0 == pTrk->GetTofDetIndex(iHH)) continue;           // exclude faked hits 
       //dTt+=(pTrk->GetTofHitPointer(iHH)->GetTime()-pTrk->GetTofHitPointer(iHL)->GetTime())/(pTrk->GetTofHitPointer(iHH)->GetR()-pTrk->GetTofHitPointer(iHL)->GetR()); // for projective geometries only !!!
       dTx+=(pTrk->GetTofHitPointer(iHH)->GetX()-pTrk->GetTofHitPointer(iHL)->GetX())/( pTrk->GetTofHitPointer(iHH)->GetZ()-pTrk->GetTofHitPointer(iHL)->GetZ() );
       iNref++;
     }	 
    }
      dTx /= iNref;
    }else{
    dTx=pTrk->GetTrackParameter()->GetTx();
    }
     
    iNref=0;
    for(Int_t iHit = 0; iHit < pTrk->GetNofHits(); iHit++) {
    if(iDetId == pTrk->GetTofDetIndex(iHit) || 0 == pTrk->GetTofDetIndex(iHit)) continue;

    Double_t dDZ = pHit->GetZ() - pTrk->GetTofHitPointer(iHit)->GetZ();
    dXref += pTrk->GetTofHitPointer(iHit)->GetX() + dTx*dDZ;
    iNref++;
  }

  if(iNref == 0) {
    LOG(error) << "DetId "<<iDetId<<", Nref "<<iNref
	 <<" sizes "<<pTrk->GetNofHits()<<", "<<pTrk->GetNofHits();
    return 1.E20;
  }

  dXref /= iNref;

  return pHit->GetX() - dXref;
}

Double_t CbmTofTrackletTools::GetYdif(CbmTofTracklet *pTrk, Int_t iDetId, CbmTofHit* pHit) {
  Double_t dYref = 0.;
  Int_t iNref = 0;
  Double_t dTy=0;
  
  if(1) {
    for (Int_t iHL=0; iHL<pTrk->GetNofHits()-1; iHL++) {
     if (iDetId == pTrk->GetTofDetIndex(iHL) || 0 == pTrk->GetTofDetIndex(iHL)) continue;           // exclude faked hits 
     for (Int_t iHH=iHL+1; iHH<pTrk->GetNofHits(); iHH++) {
       if (iDetId == pTrk->GetTofDetIndex(iHH) || 0 == pTrk->GetTofDetIndex(iHH)) continue;           // exclude faked hits 
       dTy+=(pTrk->GetTofHitPointer(iHH)->GetY()-pTrk->GetTofHitPointer(iHL)->GetY())/( pTrk->GetTofHitPointer(iHH)->GetZ()-pTrk->GetTofHitPointer(iHL)->GetZ() );
       iNref++;
     }	 
    }
      dTy /= iNref;
    }else{
      dTy=pTrk->GetTrackParameter()->GetTy();
    }
  
  iNref=0;
  for(Int_t iHit = 0; iHit < pTrk->GetNofHits(); iHit++) {
    if(iDetId == pTrk->GetTofDetIndex(iHit) || 0 == pTrk->GetTofDetIndex(iHit)) continue;

    Double_t dDZ = pHit->GetZ() - pTrk->GetTofHitPointer(iHit)->GetZ();
    dYref += pTrk->GetTofHitPointer(iHit)->GetY() + dTy*dDZ;
    iNref++;
  }

  if(iNref == 0) {
    LOG(error) << "DetId "<<iDetId<<", Nref "<<iNref
	 <<" sizes "<<pTrk->GetNofHits()<<", "<<pTrk->GetNofHits();
    return 1.E20;
  }

  dYref /= iNref;

  return pHit->GetY() - dYref;
}

Double_t CbmTofTrackletTools::GetTdif(CbmTofTracklet *pTrk, Int_t iDetId, CbmTofHit* pHit){
  Double_t dTref=0.;
  Double_t Nref=0;
  Double_t dTt=0.;
  Int_t iNt=0;
  
  if(0) {
    for (Int_t iHL=0; iHL<pTrk->GetNofHits()-1; iHL++){
     if (iDetId == pTrk->GetTofDetIndex(iHL) || 0 == pTrk->GetTofDetIndex(iHL)) continue;           // exclude faked hits 
     for (Int_t iHH=iHL+1; iHH<pTrk->GetNofHits(); iHH++){
       if (iDetId == pTrk->GetTofDetIndex(iHH) || 0 == pTrk->GetTofDetIndex(iHH)) continue;           // exclude faked hits 
       //dTt+=(pTrk->GetTofHitPointer(iHH)->GetTime()-pTrk->GetTofHitPointer(iHL)->GetTime())/(pTrk->GetTofHitPointer(iHH)->GetR()-pTrk->GetTofHitPointer(iHL)->GetR()); // for projective geometries only !!!
       Double_t dSign=1.;
       if(pTrk->GetTofHitPointer(iHH)->GetZ() < pTrk->GetTofHitPointer(iHL)->GetZ()) dSign=-1.; 
       dTt+=(pTrk->GetTofHitPointer(iHH)->GetTime()-pTrk->GetTofHitPointer(iHL)->GetTime())/pTrk->Dist3D(pTrk->GetTofHitPointer(iHH),pTrk->GetTofHitPointer(iHL))*dSign;
       iNt++;
     }
    }
  
    if (iNt==0) {
      LOG(error) << "No valid hit pair ";
      return 1.E20; 
    }
    dTt/=(Double_t)iNt;
  } else {
    dTt=FitTt(pTrk, iDetId);
  }
  
  for (Int_t iHit=0; iHit<pTrk->GetNofHits(); iHit++){
    if (iDetId == pTrk->GetTofDetIndex(iHit) || 0 == pTrk->GetTofDetIndex(iHit)) continue;
    //dTref += pTrk->GetTofHitPointer(iHit)->GetTime() - dTt*(pTrk->GetTofHitPointer(iHit)->GetR()-pHit->GetR());
    Double_t dSign=1.;
    if(pTrk->GetTofHitPointer(iHit)->GetZ()<pHit->GetZ()) dSign=-1; 
    dTref += pTrk->GetTofHitPointer(iHit)->GetTime() - dTt*dSign*pTrk->Dist3D(pTrk->GetTofHitPointer(iHit),pHit);
    Nref++;
  }
  if(Nref == 0) {
    LOG(error) << "DetId "<<iDetId<<", Nref "<<Nref
	       <<" sizes "<<pTrk->GetNofHits();
    return 1.E20;
  }
  dTref /= (Double_t)Nref;
  Double_t dTdif=pHit->GetTime()-dTref;
  // LOG(debug) << "iSt "<< iSt<<" DetId "<<iDetId<<", Nref "<<Nref<<" Tdif
  // "<<dTdif;
  return dTdif;
}

Double_t CbmTofTrackletTools::GetTexpected(CbmTofTracklet *pTrk, Int_t iDetId, CbmTofHit* pHit){
  Double_t dTref=0.;
  Double_t Nref=0;
  Double_t dTt=0.;

  dTt=FitTt(pTrk, iDetId);

  for (Int_t iHit=0; iHit<pTrk->GetNofHits(); iHit++){
    if (iDetId == pTrk->GetTofDetIndex(iHit) || 0 == pTrk->GetTofDetIndex(iHit)) continue;
    Double_t dSign=1.;
    if(pTrk->GetTofHitPointer(iHit)->GetZ()<pHit->GetZ()) dSign=-1; 
    dTref += pTrk->GetTofHitPointer(iHit)->GetTime() - dTt*dSign*pTrk->Dist3D(pTrk->GetTofHitPointer(iHit),pHit);
    Nref++;
  }
  if(Nref == 0) {
    LOG(error) << "DetId "<<iDetId<<", Nref "<< Nref  <<" sizes "<<pTrk->GetNofHits();
    return 1.E20;
  }
  dTref /= (Double_t)Nref;
  return dTref;
}

ClassImp(CbmTofTrackletTools)

