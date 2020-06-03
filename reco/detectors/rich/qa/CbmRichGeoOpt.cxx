/**
 * \file CbmRichGeoOpt.cxx
 *
 * \author Tariq Mahmoud <t.mahmoud@gsi.de>
 * \date 2014
 **/

#include "CbmRichGeoOpt.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TClonesArray.h"

#include "CbmMCTrack.h"
#include "FairTrackParam.h"
#include "CbmDrawHist.h"
#include "CbmTrackMatchNew.h"
#include "CbmRichRing.h"
#include "CbmRichHit.h"
#include "CbmRichGeoManager.h"

#include <iostream>
#include <string>
#include <boost/assign/list_of.hpp>
#include "CbmRichRingLight.h"

using namespace std;
using boost::assign::list_of;

CbmRichGeoOpt::CbmRichGeoOpt()
  : FairTask("CbmRichGeoOpt"),
    fRichPoints(NULL),
    fMcTracks(NULL),
    fRefPoints(NULL),
    fRichHits(NULL),
    fRichRings(NULL), 
    fRichRingMatches(NULL),
    fEventNum(0),
    PMTPointsFilled(0),
    fMinNofHits(7),
    nPhotonsNotOnPlane(0),
    nPhotonsNotOnSphere(0),
    nTotalPhorons(0),
    PMTPlanePoints(),
    PMTPlaneCenter(),
    ReadPMTPlaneCenter(),
    ReadPMTPlaneCenterOrig(),
    MirrorCenter(),
// ReadMirrorCenter(),
    RotX(0.),
    RotY(0.),
    PMT_r1(),
    PMT_r2(),
    PMT_norm(),
    PMTPlaneCenterX(0.),
    PMTPlaneCenterY(0.),
    PMTPlaneThirdX(0.),
    MirrPosition(),    
    SensPointsFilled(0),
    SensPlanePoints(),
    Sens_r1(),
    Sens_r2(),
    Sens_norm(),    
    H_Diff_LineRefPMT_MomAtPMT(NULL),
    H_Theta_TwoVectors(NULL),
    H_DistancePMTtoMirrCenter(NULL),
    H_DistancePMTtoMirr(NULL),
    H_MomPrim(NULL),
    H_PtPrim(NULL),
    H_MomPt(NULL),
    H_Mom_Theta_MC(NULL),
    H_Pt_Theta_MC(NULL),
    H_Mom_Theta_Rec(NULL),
    H_Pt_Theta_Rec(NULL),
    H_Mom_Theta_Acc(NULL),
    H_Pt_Theta_Acc(NULL),
    H_MomRing(NULL),
    H_acc_mom_el(NULL),
    H_acc_pty_el(NULL),
    H_Mom_XY_Theta25(NULL),
    H_MomPrim_RegularTheta(NULL),
    H_acc_mom_el_RegularTheta(NULL),
    H_Hits_XY(NULL),
    H_Hits_XY_LeftHalf(NULL),
    H_Hits_XY_RightHalf(NULL),
    H_Hits_XY_Left2Thirds(NULL),
    H_Hits_XY_RightThird(NULL),
    H_PointsIn_XY(NULL),
    H_PointsIn_XY_LeftHalf(NULL),
    H_PointsIn_XY_RightHalf(NULL),
    H_PointsIn_XY_Left2Thirds(NULL),
    H_PointsIn_XY_RightThird(NULL),
    H_PointsOut_XY(NULL),
    H_NofPhotonsPerEv(NULL),
    H_NofPhotonsPerHit(NULL),
    H_NofPhotonsSmallerThan30(NULL),
    H_DiffXhit(NULL),
    H_DiffYhit(NULL),
    H_dFocalPoint_Delta(NULL),
    H_dFocalPoint_Rho(NULL),
    H_Alpha(NULL),
    H_Alpha_UpLeft(NULL),
    H_Alpha_UpLeft_II(NULL),
    H_Alpha_UpLeft_RegularTheta(NULL),
    H_Alpha_UpLeft_LeftHalf(NULL),
    H_Alpha_UpLeft_RightHalf(NULL),
    H_Alpha_UpLeft_RightThird(NULL),
    H_Alpha_UpLeft_Left2Thirds(NULL),
    H_Alpha_XYposAtDet(NULL),
    H_Alpha_XYposAtDet_RegularTheta(NULL),
    H_Alpha_XYposAtDet_LeftHalf(NULL),
    H_Alpha_XYposAtDet_RightHalf(NULL),
    H_Alpha_XYposAtDet_RightThird(NULL),
    H_Alpha_XYposAtDet_Left2Thirds(NULL),
    H_NofHitsAll(NULL),
    H_NofRings(NULL),
    H_NofRings_NofHits(NULL),
    H_RingCenterX(NULL),
    H_RingCenterY(NULL),
    H_RingCenter(NULL),
    H_Radius(NULL), 
    H_aAxis(NULL), 
    H_bAxis(NULL), 
    H_boa(NULL), 
    H_boa_RegularTheta(NULL), 
    H_boa_LeftHalf(NULL), 
    H_boa_RightHalf(NULL), 
    H_boa_RightThird(NULL), 
    H_boa_Left2Thirds(NULL), 
    H_dR(NULL),
    H_dR_aa(NULL),
    H_dR_RegularTheta(NULL),
    H_dR_LeftHalf(NULL), 
    H_dR_RightHalf(NULL), 
    H_dR_RightThird(NULL), 
    H_dR_Left2Thirds(NULL), 
    H_RingCenter_Aaxis(NULL),  
    H_RingCenter_Baxis(NULL),  
    H_RingCenter_boa(NULL),
    H_RingCenter_boa_RegularTheta(NULL),
    H_RingCenter_boa_LeftHalf(NULL),
    H_RingCenter_boa_RightHalf(NULL),
    H_RingCenter_boa_RightThird(NULL),
    H_RingCenter_boa_Left2Thirds(NULL),
    H_RingCenter_dR(NULL),
    H_RingCenter_dR_RegularTheta(NULL),
    H_RingCenter_dR_LeftHalf(NULL),
    H_RingCenter_dR_RightHalf(NULL),
    H_RingCenter_dR_RightThird(NULL),
    H_RingCenter_dR_Left2Thirds(NULL)
    
{
  /*
    fEventNum = 0;
    PMTPointsFilled = 0;
    fMinNofHits = 7;
    nPhotonsNotOnPlane = 0;
    nPhotonsNotOnSphere = 0;
    nTotalPhorons = 0; 
  */
}

CbmRichGeoOpt::~CbmRichGeoOpt()
{
  
}

InitStatus CbmRichGeoOpt::Init()
{
   cout << "CbmRichGeoOpt::Init"<<endl;
   FairRootManager* ioman = FairRootManager::Instance();
   if (NULL == ioman) { Fatal("CbmRichGeoOpt::Init","RootManager not instantised!"); }

   fRichPoints = (TClonesArray*) ioman->GetObject("RichPoint");
   if ( NULL == fRichPoints) { Fatal("CbmRichGeoOpt::Init","No RichPoint array!"); }

   fRefPoints = (TClonesArray*) ioman->GetObject("RefPlanePoint");
   if ( NULL == fRefPoints) { Fatal("CbmRichGeoOpt::Init","No fRefPoints array!"); }

   fMcTracks = (TClonesArray*) ioman->GetObject("MCTrack");
   if ( NULL == fMcTracks) { Fatal("CbmRichGeoOpt::Init","No MCTrack array!"); }

   fRichHits = (TClonesArray*) ioman->GetObject("RichHit");
   if ( NULL == fRichHits) { Fatal("CbmRichGeoTest::Init","No RichHit array!"); }
   
   ///////////////////////////////////////////
   fRichRings = (TClonesArray*) ioman->GetObject("RichRing");
   if ( NULL == fRichRings) { Fatal("CbmRichGeoTest::Init","No RichRing array!"); }

   fRichRingMatches = (TClonesArray*) ioman->GetObject("RichRingMatch");
   if ( NULL == fRichRingMatches) { Fatal("CbmRichGeoTest::Init","No RichRingMatch array!"); }

   /////////////// need three points on the PMT plane to determine its equation
   //cout<<" initializing points values -1000"<<endl;
   PMTPlanePoints.resize(3);
   SensPlanePoints.resize(3);
   for(unsigned int p=0;p<PMTPlanePoints.size();p++){
     PMTPlanePoints[p].SetX(-1000.);PMTPlanePoints[p].SetY(-1000.);PMTPlanePoints[p].SetZ(-1000.);
     SensPlanePoints[p].SetX(-1000.);SensPlanePoints[p].SetY(-1000.);SensPlanePoints[p].SetZ(-1000.);
   }
   //   CbmRichRecGeoPar* gp = CbmRichGeoManager::GetInstance().fGP;
    PMTPlaneCenterX = 0.;//-1.*gp->fPmtWidthX/2.;
    PMTPlaneCenterY = 0.;//-1.*gp->fPmtWidthY/2.;
    PMTPlaneThirdX =0.;//-1.*gp->fPmtWidthX/3.;
   //cout<<"fGP.fPmtWidthX = "<< gp->fPmtWidthX<<",  PMTPlaneCenterX = "<< PMTPlaneCenterX<<",  PMTPlaneThirdX = "<< PMTPlaneThirdX<<endl;
 
   InitHistograms();
   return kSUCCESS;
}

void CbmRichGeoOpt::Exec(Option_t* /*option*/)
{
  fEventNum++;
  // cout << "#################### CbmRichGeoOpt, event No. " <<  fEventNum << endl;
  
  if(PMTPointsFilled==0){
    for(unsigned int p=1;p<PMTPlanePoints.size();p++){
      if( PMTPlanePoints[p].X() == PMTPlanePoints[p-1].X() ){
	cout<<"PMTPlanePoints["<<p<<"].X == PMTPlanePoints["<<p-1<<"].X =="<<PMTPlanePoints[p-1].X()<<endl;
	cout<<"Calling FillPointsAtPMT()"<<endl;
	FillPointsAtPMT();PMTPointsFilled=0;
      }else{
	PMTPointsFilled=1;
      }
    }
  }
  if(SensPointsFilled==0){
    for(unsigned int p=1;p<SensPlanePoints.size();p++){
      if( SensPlanePoints[p].X() == SensPlanePoints[p-1].X() ){FillPointsAtPMTSensPlane();SensPointsFilled=0;}else{SensPointsFilled=1;}
    }
  }
 
  //cout << "#################### CbmRichGeoOpt, event No. " <<  fEventNum << endl;
  //Fill the coordinates of the three points on the PMT plane 
  CbmRichRecGeoPar* gp = CbmRichGeoManager::GetInstance().fGP;
  PMTPlaneCenter.SetX(gp->fPmt.fX); PMTPlaneCenter.SetY(gp->fPmt.fY); PMTPlaneCenter.SetZ(gp->fPmt.fZ);
  /////////////////////////////////////////
  // if(PMTPointsFilled==1 && SensPointsFilled==1 ){cout<<" Both are filled."<<endl;}
  // else if(PMTPointsFilled==1 && SensPointsFilled==0){cout<<" PMTPointsFilled==1 && SensPointsFilled==0."<<endl;}
  // else if(PMTPointsFilled==0 && SensPointsFilled==1){cout<<" PMTPointsFilled==0 && SensPointsFilled==1."<<endl;}
  // else{cout<<" PMTPointsFilled==0 && SensPointsFilled==0."<<endl;}

  if(PMTPointsFilled==1){
    if(fEventNum<10){
      for(unsigned int p=0;p<PMTPlanePoints.size();p++){
	cout<<"Point "<<p<< ": ("<<PMTPlanePoints[p].X()<<" , "<< PMTPlanePoints[p].Y()<<" , "<< PMTPlanePoints[p].Z()<<")"<<endl;
      }
      
      PMT_r1=PMTPlanePoints[1]-PMTPlanePoints[0]; 
      PMT_r2=PMTPlanePoints[2]-PMTPlanePoints[0]; 
      PMT_norm=PMT_r1.Cross(PMT_r2);
      MirrorCenter.SetX(gp->fMirrorX);MirrorCenter.SetY(gp->fMirrorY);MirrorCenter.SetZ(gp->fMirrorZ);
      cout<<"MirrorCenter=("<<MirrorCenter.X()<<","<<MirrorCenter.Y()<<","<<MirrorCenter.Z()<<")"<<endl;
      cout<<"PMT_r1=("<<PMT_r1.X()<<","<<PMT_r1.Y()<<","<<PMT_r1.Z()<<")"<<endl;
      cout<<"PMT_r2=("<<PMT_r2.X()<<","<<PMT_r2.Y()<<","<<PMT_r2.Z()<<")"<<endl;
      cout<<"PMT_norm=("<<PMT_norm.X()<<","<<PMT_norm.Y()<<","<<PMT_norm.Z()<<")"<<endl;
    }
    // //HitsAndPointsWithRef();
    HitsAndPoints();
    RingParameters();
    FillMcHist();
  }
  //fGP.Print();
  // cout<<" ========== PMT_TRAZ = "<<TString(gSystem->Getenv("PMT_TRAZ")).Atof()<<endl;
  // cout<<" ========== PMT_ROTX = "<<TString(gSystem->Getenv("PMT_ROTX")).Atof()<<endl;
}
/////// Get data of hits and points
void CbmRichGeoOpt::HitsAndPoints(){
  //***********************************************************  
  //**** points
  //***************
  Int_t nofPoints = fRichPoints->GetEntriesFast();
  if(nofPoints<0 || nofPoints>2000){return;} 
  H_NofPhotonsPerEv->Fill(nofPoints); 
  for(Int_t ip = 0; ip < nofPoints; ip++){
    TVector3 PosAtDetIn; TVector3 PosAtDetOut;
    CbmRichPoint* point = (CbmRichPoint*) fRichPoints->At(ip);
    if(NULL == point) continue;
    //int trackId = point->GetTrackID(); if(trackId<0) continue;
    PosAtDetIn.SetX(point->GetX()); PosAtDetIn.SetY(point->GetY()); PosAtDetIn.SetZ(point->GetZ());
    bool Checked=CheckPointLiesOnPlane(PosAtDetIn,PMTPlanePoints[0],PMT_norm);
    if(!Checked) continue;
    H_PointsIn_XY->Fill(PosAtDetIn.X(),PosAtDetIn.Y());
    if(PosAtDetIn.X() <= PMTPlaneCenterX){H_PointsIn_XY_LeftHalf->Fill(PosAtDetIn.X(),PosAtDetIn.Y()); }
    if(PosAtDetIn.X() > PMTPlaneCenterX){ H_PointsIn_XY_RightHalf->Fill(PosAtDetIn.X(),PosAtDetIn.Y()); }
    if(PosAtDetIn.X() <= PMTPlaneThirdX){ H_PointsIn_XY_Left2Thirds->Fill(PosAtDetIn.X(),PosAtDetIn.Y()); }
    if(PosAtDetIn.X() > PMTPlaneThirdX){  H_PointsIn_XY_RightThird->Fill(PosAtDetIn.X(),PosAtDetIn.Y()); }    
    
    CbmRichGeoManager::GetInstance().RotatePoint(&PosAtDetIn, &PosAtDetOut);
    H_PointsOut_XY->Fill(PosAtDetOut.X(),PosAtDetOut.Y());
    
    /////////
    TVector3 MomAtPMT; MomAtPMT.SetX(point->GetPx()); MomAtPMT.SetY(point->GetPy()); MomAtPMT.SetZ(point->GetPz());
    //    float MagMomAtPMT=MomAtPMT.Mag(); 
    
    Int_t PointMCTrackId = point->GetTrackID(); if(PointMCTrackId<0) continue;
    CbmMCTrack* PointTrack = static_cast<CbmMCTrack*>(fMcTracks->At(PointMCTrackId));
    if (NULL == PointTrack) continue;
    TVector3 PointMom; PointTrack->GetMomentum(PointMom);
    
    Int_t PointMotherId = PointTrack->GetMotherId();
    if (PointMotherId == -1){continue;}
    CbmMCTrack* motherTrack = static_cast<CbmMCTrack*>(fMcTracks->At(PointMotherId));
    int pdg = TMath::Abs(motherTrack->GetPdgCode());
    int motherId = motherTrack->GetMotherId();
    TVector3 ElMom; Double_t ElTheta = 0.;
    if (pdg == 11 && motherId == -1){motherTrack->GetMomentum(ElMom);   ElTheta=ElMom.Theta()* 180 / TMath::Pi();}
    double Alpha2=MomAtPMT.Angle(PMT_norm);//*TMath::RadToDeg();
    double Alpha2InDeg=Alpha2*TMath::RadToDeg();
    if(Alpha2InDeg>90.){Alpha2InDeg=180.-Alpha2InDeg;}
     H_Alpha->Fill(Alpha2InDeg);
    if(PosAtDetIn.X()<0. && PosAtDetIn.Y()>0) {
      if(ElTheta<=25){
	H_Alpha_UpLeft_RegularTheta->Fill(Alpha2InDeg );
	H_Alpha_XYposAtDet_RegularTheta->Fill(PosAtDetIn.X(),PosAtDetIn.Y(),Alpha2InDeg);
      }
      H_Alpha_UpLeft->Fill(Alpha2InDeg );
      H_Alpha_XYposAtDet->Fill(PosAtDetIn.X(),PosAtDetIn.Y(),Alpha2InDeg);
      
      if(PosAtDetIn.X() <= PMTPlaneCenterX){H_Alpha_UpLeft_LeftHalf->Fill(Alpha2InDeg ); H_Alpha_XYposAtDet_LeftHalf->Fill(PosAtDetIn.X(),PosAtDetIn.Y(),Alpha2InDeg);}
      if(PosAtDetIn.X() > PMTPlaneCenterX){H_Alpha_UpLeft_RightHalf->Fill(Alpha2InDeg ); H_Alpha_XYposAtDet_RightHalf->Fill(PosAtDetIn.X(),PosAtDetIn.Y(),Alpha2InDeg);}
      if(PosAtDetIn.X() <= PMTPlaneThirdX){
	H_Alpha_UpLeft_Left2Thirds->Fill(Alpha2InDeg ); H_Alpha_XYposAtDet_Left2Thirds->Fill(PosAtDetIn.X(),PosAtDetIn.Y(),Alpha2InDeg);
      }
      if(PosAtDetIn.X() > PMTPlaneThirdX){
	H_Alpha_UpLeft_RightThird->Fill(Alpha2InDeg ); H_Alpha_XYposAtDet_RightThird->Fill(PosAtDetIn.X(),PosAtDetIn.Y(),Alpha2InDeg);
      }
    }

    
  }//end loop points
 

  //***********************************************************  
  //**** Hits
  //***************
  Int_t nofHits = fRichHits->GetEntriesFast();

  for (Int_t iH = 0; iH < nofHits; iH++){
    CbmRichHit *hit = (CbmRichHit*) fRichHits->At(iH); if ( hit == NULL ) continue;
    Int_t pointInd =  hit->GetRefId(); if (pointInd < 0) continue;
    CbmRichPoint *point = (CbmRichPoint*) fRichPoints->At(pointInd); if ( point == NULL ) continue;
    
  //  H_NofPhotonsPerHit->Fill(hit->GetNPhotons());
    
    TVector3 inPos(point->GetX(), point->GetY(), point->GetZ());
    TVector3 outPos;
    CbmRichGeoManager::GetInstance().RotatePoint(&inPos, &outPos);
    H_Hits_XY->Fill(hit->GetX(), hit->GetY());

    if(hit->GetX() <= PMTPlaneCenterX){H_Hits_XY_LeftHalf->Fill(hit->GetX(), hit->GetY()); }
    if(hit->GetX() > PMTPlaneCenterX){ H_Hits_XY_RightHalf->Fill(hit->GetX(), hit->GetY()); }
    if(hit->GetX() <= PMTPlaneThirdX){ H_Hits_XY_Left2Thirds->Fill(hit->GetX(), hit->GetY()); }
    if(hit->GetX() > PMTPlaneThirdX){  H_Hits_XY_RightThird->Fill(hit->GetX(), hit->GetY()); }

    H_DiffXhit->Fill(hit->GetX() - outPos.X());
    H_DiffYhit->Fill(hit->GetY() - outPos.Y());
    
  }
}

/////// Get data of hits and points
void CbmRichGeoOpt::HitsAndPointsWithRef(){
  Int_t nofRefPoints = fRefPoints->GetEntriesFast();
  Int_t nofPoints = fRichPoints->GetEntriesFast();
  if(nofPoints==0 || nofRefPoints==0){return;} if(nofPoints>2000){return;} 
  
  for (int i = 0; i < nofRefPoints; i++) {
    TVector3 PosAtRefl; TVector3 PosAtDetIn; 
    CbmRichPoint* RefPoint = (CbmRichPoint*)fRefPoints->At(i);
    TVector3 MomAtRef; MomAtRef.SetX(RefPoint->GetPx()); MomAtRef.SetY(RefPoint->GetPy()); MomAtRef.SetZ(RefPoint->GetPz()); //RefPoint->GetMomentum(MomAtRef);
    
    if (RefPoint == NULL) continue;
    int RefPointTrackId = RefPoint->GetTrackID(); if(RefPointTrackId<0) {continue;}
    CbmMCTrack* RefPointTrack = static_cast<CbmMCTrack*>(fMcTracks->At(RefPointTrackId));
    int pdg0 = RefPointTrack->GetPdgCode(); if(TMath::Abs(pdg0) == 11) {continue;}
    RefPoint->Position(PosAtRefl);
    int Zpos=int(10.*PosAtRefl.Z());//3037 0r 3038  -->take 3038 which is the entrance point 
    //of the REFLECTED photon into the sensitive plane   
    cout<<PosAtRefl.Z()<<"    "<<Zpos<<endl;
    //if(Zpos==3037){continue;}
    TVector3 momRef; RefPointTrack->GetMomentum(momRef);  

    CbmRichPoint* point = GetPMTPoint(RefPointTrackId);//
    PosAtDetIn.SetX(point->GetX()); PosAtDetIn.SetY(point->GetY()); PosAtDetIn.SetZ(point->GetZ());
    TVector3 MomAtPMT; MomAtPMT.SetX(point->GetPx()); MomAtPMT.SetY(point->GetPy()); MomAtPMT.SetZ(point->GetPz());
    float MagMomAtPMT=MomAtPMT.Mag(); 
   //point->GetMomentum(MomAtPMT);

    Int_t PointMCTrackId = point->GetTrackID();
    CbmMCTrack* PointTrack = static_cast<CbmMCTrack*>(fMcTracks->At(PointMCTrackId));
    if (NULL == PointTrack) continue;
    TVector3 PointMom; PointTrack->GetMomentum(PointMom);
    
    Int_t PointMotherId = PointTrack->GetMotherId();
    if (PointMotherId == -1){continue;}

    CbmMCTrack* motherTrack = static_cast<CbmMCTrack*>(fMcTracks->At(PointMotherId));
    int pdg = TMath::Abs(motherTrack->GetPdgCode());
    int motherId = motherTrack->GetMotherId();
    TVector3 ElMom; Double_t ElTheta = 0.; //float ElMomentum=0.;
    if (pdg == 11 && motherId == -1){
      motherTrack->GetMomentum(ElMom);   ElTheta=ElMom.Theta()* 180 / TMath::Pi(); //ElMomentum=motherTrack->GetP();
      //cout<<"ElTheta = "<<ElTheta<<", ElMomentum = "<<ElMomentum<<endl;
    }
    
    ////////////////////////////////////////////////////
    bool Checked=CheckPointLiesOnPlane(PosAtDetIn,PMTPlanePoints[0],PMT_norm);
    if(!Checked) continue;//cout<<" point not on plane: ("<<point->GetX()<<","<<point->GetY()<<","<<point->GetZ()<<")"<<endl; continue;

    TVector3 LineSensToPMT=PosAtDetIn-PosAtRefl;
    float MagLineSensToPMT=LineSensToPMT.Mag(); 
    H_Diff_LineRefPMT_MomAtPMT->Fill(MagLineSensToPMT-MagMomAtPMT);

    H_Theta_TwoVectors->Fill(LineSensToPMT.Angle(MomAtPMT));
    /////////// calculate alpha relative to the "tilted" PMT plane !!
    double Alpha=LineSensToPMT.Angle(PMT_norm);//*TMath::RadToDeg();
    double AlphaInDeg=Alpha*TMath::RadToDeg();
    if(AlphaInDeg>90.){AlphaInDeg=180.-AlphaInDeg;}
    /////////// calculate alpha throuh the momentum vector !!
    //double Alpha2=PointMom.Angle(PMT_norm);//*TMath::RadToDeg();
    double Alpha2=MomAtPMT.Angle(PMT_norm);//*TMath::RadToDeg();
    double Alpha2InDeg=Alpha2*TMath::RadToDeg();
    if(Alpha2InDeg>90.){Alpha2InDeg=180.-Alpha2InDeg;}
    //cout<<PointMom.X()<<"  "<<MomAtPMT.X()<<"   "<<MomAtRef.X()<<"   "<<Alpha<<"   "<<Alpha2<<endl;

    //PosAtDetOut
  

    H_Alpha->Fill(Alpha2InDeg);
    if(PosAtDetIn.X()<0. && PosAtDetIn.Y()>0) {
      if(ElTheta<=25){
	H_Alpha_UpLeft_RegularTheta->Fill(Alpha2InDeg );
	H_Alpha_XYposAtDet_RegularTheta->Fill(PosAtDetIn.X(),PosAtDetIn.Y(),Alpha2InDeg);
      }
      H_Alpha_UpLeft->Fill(Alpha2InDeg );
      H_Alpha_UpLeft_II->Fill(AlphaInDeg );
      H_Alpha_XYposAtDet->Fill(PosAtDetIn.X(),PosAtDetIn.Y(),Alpha2InDeg);

      if(PosAtDetIn.X() <= PMTPlaneCenterX){H_Alpha_UpLeft_LeftHalf->Fill(Alpha2InDeg ); H_Alpha_XYposAtDet_LeftHalf->Fill(PosAtDetIn.X(),PosAtDetIn.Y(),Alpha2InDeg);}
      if(PosAtDetIn.X() > PMTPlaneCenterX){H_Alpha_UpLeft_RightHalf->Fill(Alpha2InDeg ); H_Alpha_XYposAtDet_RightHalf->Fill(PosAtDetIn.X(),PosAtDetIn.Y(),Alpha2InDeg);}
      if(PosAtDetIn.X() <= PMTPlaneThirdX){
	H_Alpha_UpLeft_Left2Thirds->Fill(Alpha2InDeg ); H_Alpha_XYposAtDet_Left2Thirds->Fill(PosAtDetIn.X(),PosAtDetIn.Y(),Alpha2InDeg);
      }
      if(PosAtDetIn.X() > PMTPlaneThirdX){
	H_Alpha_UpLeft_RightThird->Fill(Alpha2InDeg ); H_Alpha_XYposAtDet_RightThird->Fill(PosAtDetIn.X(),PosAtDetIn.Y(),Alpha2InDeg);
      }
    }
  }
  
  //***********************************************************  
  Int_t nofHits = fRichHits->GetEntriesFast();

  for (Int_t iH = 0; iH < nofHits; iH++){
    CbmRichHit *hit = (CbmRichHit*) fRichHits->At(iH); if ( hit == NULL ) continue;
    Int_t pointInd =  hit->GetRefId(); if (pointInd < 0) continue;
    CbmRichPoint *point = (CbmRichPoint*) fRichPoints->At(pointInd); if ( point == NULL ) continue;
    
  //  H_NofPhotonsPerHit->Fill(hit->GetNPhotons());
    
    TVector3 inPos(point->GetX(), point->GetY(), point->GetZ());
    TVector3 outPos;
    CbmRichGeoManager::GetInstance().RotatePoint(&inPos, &outPos);
    H_Hits_XY->Fill(hit->GetX(), hit->GetY());

    if(hit->GetX() <= PMTPlaneCenterX){H_Hits_XY_LeftHalf->Fill(hit->GetX(), hit->GetY()); }
    if(hit->GetX() > PMTPlaneCenterX){ H_Hits_XY_RightHalf->Fill(hit->GetX(), hit->GetY()); }
    if(hit->GetX() <= PMTPlaneThirdX){ H_Hits_XY_Left2Thirds->Fill(hit->GetX(), hit->GetY()); }
    if(hit->GetX() > PMTPlaneThirdX){  H_Hits_XY_RightThird->Fill(hit->GetX(), hit->GetY()); }

    H_DiffXhit->Fill(hit->GetX() - outPos.X());
    H_DiffYhit->Fill(hit->GetY() - outPos.Y());
    
  }
}

///////////////////////////////
void CbmRichGeoOpt::RingParameters()
{
  Int_t nofRings = fRichRings->GetEntriesFast();
  for (Int_t iR = 0; iR < nofRings; iR++)
	{
	    CbmRichRing *ring = (CbmRichRing*) fRichRings->At(iR);  if (NULL == ring) continue;
	    CbmTrackMatchNew* ringMatch = (CbmTrackMatchNew*) fRichRingMatches->At(iR); 
	    if (NULL == ringMatch)
			{ 
	      		// H_NofRings->SetBinContent(8,H_NofRings->GetBinCenter(8)+1);
	     		continue;
			}
    
    	Int_t mcTrackId = ringMatch->GetMatchedLink().GetIndex();  
    	if (mcTrackId < 0){
   	   	//H_NofRings->SetBinContent(8,H_NofRings->GetBinCenter(8)+1); 
      	continue;}//{ continue;}
    	CbmMCTrack* mcTrack = (CbmMCTrack*)fMcTracks->At(mcTrackId);  if (!mcTrack){
    	  //H_NofRings->SetBinContent(8,H_NofRings->GetBinCenter(8)+1); 
    	 continue;}// continue;
    
    	Int_t motherId = mcTrack->GetMotherId();
    	Int_t pdg = TMath::Abs(mcTrack->GetPdgCode());
    	if (pdg != 11 || motherId != -1)
			{ 
	      		//H_NofRings->SetBinContent(8,H_NofRings->GetBinCenter(8)+1); 
      			continue;
    		}// continue; // only primary electrons
    
    	Double_t momentum = mcTrack->GetP();
    	Double_t pt = mcTrack->GetPt();
    	Double_t rapidity = mcTrack->GetRapidity();
    
    	TVector3 mom; mcTrack->GetMomentum(mom);  
    	Double_t theta=mom.Theta()* 180 / TMath::Pi();
    	H_MomRing->Fill(momentum);
    	H_Mom_Theta_Rec->Fill(momentum, theta);
    	H_Pt_Theta_Rec->Fill(pt, theta);

    	// Double_t theta = mcTrack->Theta();
    	// cout<<" **************************** "<<theta<<endl;
 
    	H_NofRings->Fill(nofRings);
    	if (ring->GetNofHits() >= fMinNofHits)
			{
      			H_Mom_Theta_Acc->Fill(momentum, theta);
      			H_Pt_Theta_Acc->Fill(pt, theta);
      			H_acc_mom_el->Fill(momentum);
      			if(theta<=25)
					{
						H_acc_mom_el_RegularTheta->Fill(momentum);
					}
      
      			H_acc_pty_el->Fill(rapidity, pt);
      
    		}
    
    ///////////////////////////////////
    float radius=ring->GetRadius();
    if(radius<=0.){continue;}//with ideal finder --> many rings with radius -1. 
    //Test if radius is a NAN:
    if(! (radius<=1. || radius >1.) ){continue;}
    float aA = ring->GetAaxis(); 
    float bA = ring->GetBaxis(); 

    H_Radius->Fill(radius); H_aAxis->Fill(aA); H_bAxis->Fill(bA); H_boa->Fill(bA/aA);

    float CentX=ring->GetCenterX();
    float CentY=ring->GetCenterY();
    
    H_RingCenter->Fill(CentX,CentY); 
    H_RingCenter_Aaxis->Fill(CentX,CentY,aA);
    H_RingCenter_Baxis->Fill(CentX,CentY,bA); 
    //cout << "PMT size in x and y [cm]: " << fGP.fPmtWidthX << "  " << fGP.fPmtWidthY << endl;
    if(theta<=25 && theta>=24.9){H_Mom_XY_Theta25->Fill(CentX,CentY,momentum);}

    H_RingCenter_boa->Fill(CentX,CentY,bA/aA);
    if(theta<=25){H_boa_RegularTheta->Fill(bA/aA); H_RingCenter_boa_RegularTheta->Fill(CentX,CentY,bA/aA);}
    if(CentX > PMTPlaneCenterX){H_boa_RightHalf->Fill(bA/aA); H_RingCenter_boa_RightHalf->Fill(CentX,CentY,bA/aA);}
    if(CentX <= PMTPlaneCenterX){H_boa_LeftHalf->Fill(bA/aA); H_RingCenter_boa_LeftHalf->Fill(CentX,CentY,bA/aA);}
    if(CentX > PMTPlaneThirdX){H_boa_RightThird->Fill(bA/aA); H_RingCenter_boa_RightThird->Fill(CentX,CentY,bA/aA);}
    if(CentX <= PMTPlaneThirdX){H_boa_Left2Thirds->Fill(bA/aA); H_RingCenter_boa_Left2Thirds->Fill(CentX,CentY,bA/aA);}
    
    
    // if(CentX <=  PMTPlaneCenterX && CentY >PMTPlaneCenterY){H_boa_Q1->Fill(bA/aA);}
    int nAllHitsInR=ring->GetNofHits();
    H_NofHitsAll->Fill(nAllHitsInR);
    H_NofRings_NofHits->Fill(nofRings,nAllHitsInR);

    for(int iH=0;iH<nAllHitsInR;iH++){
      CbmRichHit* hit = (CbmRichHit*) fRichHits->At(ring->GetHit(iH));
      double xH=hit->GetX();
      double yH=hit->GetY();
      double dRaa=aA-TMath::Sqrt( (CentX-xH)*(CentX-xH) + (CentY-yH)*(CentY-yH) );
      H_dR_aa->Fill(dRaa);
      double dR=radius-TMath::Sqrt( (CentX-xH)*(CentX-xH) + (CentY-yH)*(CentY-yH) );
      H_dR->Fill(dR);
      H_RingCenter_dR->Fill(CentX,CentY,dR);
      if(theta<=25){H_dR_RegularTheta->Fill(dR); H_RingCenter_dR_RegularTheta->Fill(CentX,CentY,dR);}
      if(CentX > PMTPlaneCenterX){H_dR_RightHalf->Fill(dR); H_RingCenter_dR_RightHalf->Fill(CentX,CentY,dR);}
      if(CentX <= PMTPlaneCenterX){H_dR_LeftHalf->Fill(dR); H_RingCenter_dR_LeftHalf->Fill(CentX,CentY,dR);}
      if(CentX > PMTPlaneThirdX){H_dR_RightThird->Fill(dR); H_RingCenter_dR_RightThird->Fill(CentX,CentY,dR);}
      if(CentX <= PMTPlaneThirdX ){H_dR_Left2Thirds->Fill(dR); H_RingCenter_dR_Left2Thirds->Fill(CentX,CentY,dR);}
    } 
  }
}

////////////////////////////////
void CbmRichGeoOpt::FillMcHist()
{
  for (Int_t i = 0; i < fMcTracks->GetEntriesFast(); i++){
    CbmMCTrack* mcTrack = (CbmMCTrack*)fMcTracks->At(i);
    if (!mcTrack) continue;
    Int_t motherId = mcTrack->GetMotherId();
    Int_t pdg = TMath::Abs(mcTrack->GetPdgCode());

    TVector3 mom; mcTrack->GetMomentum(mom);  
    Double_t theta=mom.Theta()* 180 / TMath::Pi();
    
    if (pdg == 11 && motherId == -1){

      H_MomPt->Fill( mcTrack->GetP(), mcTrack->GetPt());
      H_MomPrim->Fill(mcTrack->GetP());
      H_PtPrim->Fill(mcTrack->GetPt());
      H_Mom_Theta_MC->Fill(mcTrack->GetP(), theta);
      H_Pt_Theta_MC->Fill(mcTrack->GetPt(), theta);
      if(theta<=25){H_MomPrim_RegularTheta->Fill(mcTrack->GetP());}
    }
  }

}

////////////////////////////////
void CbmRichGeoOpt::InitHistograms()
{
  //  int nBinsX = 28; double xMin = -110.; double xMax = 110.;  
  //  int nBinsY = 40; double yMin = -200; double yMax = 200.;
  
  H_Diff_LineRefPMT_MomAtPMT= new TH1D("H_Diff_LineRefPMT_MomAtPMT", "H_Diff_LineRefPMT_MomAtPMT;#Delta [cm]; Yield", 100, -10., 10.);

  H_Theta_TwoVectors= new TH1D("H_Theta_TwoVectors", "H_Theta_TwoVectors;#theta [deg]; Yield", 100, 0., 10.);
  H_MomRing = new TH1D("H_MomRing", "H_MomRing;p [GeV]; Yield", 49, 0., 12.);
  H_MomPrim = new TH1D("H_MomPrim", "H_MomPrim;p [GeV]; Yield", 49, 0., 12.);
  H_PtPrim = new TH1D("H_PtPrim", "H_PtPrim;p [GeV]; Yield", 81, 0., 4.);
  H_MomPt = new TH2D("H_MomPt", "H_MomPt;p [GeV];pt [GeV]; Yield", 101, 0., 10., 81, 0., 4.);
  H_Mom_Theta_MC = new TH2D("H_Mom_Theta_MC", "H_Mom_Theta_MC;p [GeV];theta [deg]; Yield", 40, 0., 10., 50, 0, 25.);
  H_Pt_Theta_MC = new TH2D("H_Pt_Theta_MC", "H_Pt_Theta_MC;pt [GeV];theta [deg]; Yield", 16, 0., 4., 50, 0, 25.);

  H_Mom_Theta_Rec = new TH2D("H_Mom_Theta_Rec", "H_Mom_Theta_Rec;p [GeV];theta [deg]; Yield", 40, 0., 10., 50, 0, 25.);
  H_Pt_Theta_Rec = new TH2D("H_Pt_Theta_Rec", "H_Pt_Theta_Rec;pt [GeV];theta [deg]; Yield", 16, 0., 4., 50, 0, 25.);
  H_Mom_Theta_Acc = new TH2D("H_Mom_Theta_Acc", "H_Mom_Theta_Acc;p [GeV];theta [deg]; Yield", 40, 0., 10., 50, 0, 25.);
  H_Pt_Theta_Acc = new TH2D("H_Pt_Theta_Acc", "H_Pt_Theta_Acc;pt [GeV];theta [deg]; Yield", 16, 0., 4., 50, 0, 25.);

  H_MomPrim_RegularTheta = new TH1D("H_MomPrim_RegularTheta", "H_MomPrim_RegularTheta;p [GeV]; Yield", 49, 0., 12.);
    H_acc_mom_el_RegularTheta = new TH1D("H_acc_mom_el_RegularTheta", "H_acc_mom_el_RegularTheta;p [GeV/c];Yield", 49, 0., 12.);



  H_Hits_XY = new TH2D("H_Hits_XY", "H_Hits_XY;X [cm];Y [cm];Counter",151, -150., 0.,351, 0.,350.);
  H_Hits_XY_LeftHalf = new TH2D("H_Hits_XY_LeftHalf", "H_Hits_XY_LeftHalf;X [cm];Y [cm];Counter",151, -150., 0.,351, 0.,350.);
  H_Hits_XY_RightHalf = new TH2D("H_Hits_XY_RightHalf", "H_Hits_XY_RightHalf;X [cm];Y [cm];Counter",151, -150., 0.,351, 0.,350.);
  H_Hits_XY_RightThird = new TH2D("H_Hits_XY_RightThird", "H_Hits_XY_RightThird;X [cm];Y [cm];Counter",151, -150., 0.,351, 0.,350.);
  H_Hits_XY_Left2Thirds = new TH2D("H_Hits_XY_Left2Thirds", "H_Hits_XY_Left2Thirds;X [cm];Y [cm];Counter",151, -150., 0.,351, 0.,350.);

  H_PointsIn_XY = new TH2D("H_PointsIn_XY", "H_PointsIn_XY;X [cm];Y [cm];Counter", 151, -150., 0.,251, 50.,250.);
  H_PointsIn_XY_LeftHalf = new TH2D("H_PointsIn_XY_LeftHalf", "H_PointsIn_XY_LeftHalf;X [cm];Y [cm];Counter", 151, -150., 0.,251, 50.,250.);
  H_PointsIn_XY_RightHalf = new TH2D("H_PointsIn_XY_RightHalf", "H_PointsIn_XY_RightHalf;X [cm];Y [cm];Counter", 151, -150., 0.,251, 50.,250.);
  H_PointsIn_XY_RightThird = new TH2D("H_PointsIn_XY_RightThird", "H_PointsIn_XY_RightThird;X [cm];Y [cm];Counter", 151, -150., 0.,251, 50.,250.);
  H_PointsIn_XY_Left2Thirds = new TH2D("H_PointsIn_XY_Left2Thirds", "H_PointsIn_XY_Left2Thirds;X [cm];Y [cm];Counter", 151, -150., 0.,251, 50.,250.);

  H_PointsOut_XY = new TH2D("H_PointsOut_XY", "H_PointsOut_XY;X [cm];Y [cm];Counter",151, -150., 0.,351, 0.,350.);
  //cout<<" init hist H_NofPhotonsPerEv"<<endl;
  H_NofPhotonsPerEv = new TH1D("H_NofPhotonsPerEv", "H_NofPhotonsPerEv;Number of photons per event;Yield", 500, 0., 1000.);
  H_NofPhotonsPerHit = new TH1D("H_NofPhotonsPerHit", "H_NofPhotonsPerHit;Number of photons per hit;Yield", 10, -0.5, 9.5);
  H_NofPhotonsSmallerThan30 = new TH1D("H_NofPhotonsSmallerThan30", "H_NofPhotonsSmallerThan30 ;Number of photons;Yield", 10, -0.5, 9.5);
  H_DiffXhit = new TH1D("H_DiffXhit", "H_DiffXhit;Y_{point}-Y_{hit} [cm];Yield", 200, -1., 1.);
  H_DiffYhit = new TH1D("H_DiffYhit", "H_DiffYhit;Y_{point}-Y_{hit} [cm];Yield", 200, -1., 1.);
 
  H_Alpha= new TH1D("H_Alpha","H_Alpha;#alpha_{photon-PMT} [deg];Yield",180,0.,180.);
  H_Alpha_UpLeft= new TH1D("H_Alpha_UpLeft","H_Alpha_UpLeft;#alpha_{photon-PMT} [deg];Yield",180,0.,180.);
  H_Alpha_UpLeft_II= new TH1D("H_Alpha_UpLeft_II","H_Alpha_UpLeft_II;#alpha_{photon-PMT} [deg];Yield",180,0.,180.);
  H_Alpha_UpLeft_RegularTheta= new TH1D("H_Alpha_UpLeft_RegularTheta","H_Alpha_UpLeft_RegularTheta;#alpha_{photon-PMT} [deg];Yield",180,0.,180.);

  H_Alpha_UpLeft_LeftHalf= new TH1D("H_Alpha_UpLeft_LeftHalf","H_Alpha_UpLeft_LeftHalf;#alpha_{photon-PMT} [deg];Yield",180,0.,180.);
  H_Alpha_UpLeft_RightHalf= new TH1D("H_Alpha_UpLeft_RightHalf","H_Alpha_UpLeft_RightHalf;#alpha_{photon-PMT} [deg];Yield",180,0.,180.);
  H_Alpha_UpLeft_Left2Thirds= new TH1D("H_Alpha_UpLeft_Left2Thirds","H_Alpha_UpLeft_Left2Thirds;#alpha_{photon-PMT} [deg];Yield",180,0.,180.);
  H_Alpha_UpLeft_RightThird= new TH1D("H_Alpha_UpLeft_RightThird","H_Alpha_UpLeft_RightThird;#alpha_{photon-PMT} [deg];Yield",180,0.,180.);
 
  //cout<<" init hist H_Alpha_XYposAtDet"<<endl;
  H_Alpha_XYposAtDet= new TH3D("H_Alpha_XYposAtDet","H_Alpha_XYposAtDet; X [cm]; Y [cm];#alpha_{photon-PMT} [deg];Yield",151, -150., 0.,  251, 50,250, 180,0.,180.);
  H_Alpha_XYposAtDet_RegularTheta= new TH3D("H_Alpha_XYposAtDet_RegularTheta","H_Alpha_XYposAtDet_RegularTheta; X [cm]; Y [cm];#alpha_{photon-PMT} [deg];Yield",151, -150., 0.,  251, 50,250, 180,0.,180.);
  H_Alpha_XYposAtDet_LeftHalf= new TH3D("H_Alpha_XYposAtDet_LeftHalf","H_Alpha_XYposAtDet_LeftHalf; X [cm]; Y [cm];#alpha_{photon-PMT} [deg];Yield",151, -150., 0.,  251, 50,250, 180,0.,180.);
  H_Alpha_XYposAtDet_RightHalf= new TH3D("H_Alpha_XYposAtDet_RightHalf","H_Alpha_XYposAtDet_RightHalf; X [cm]; Y [cm];#alpha_{photon-PMT} [deg];Yield",151, -150., 0.,  251, 50,250, 180,0.,180.);
  H_Alpha_XYposAtDet_Left2Thirds= new TH3D("H_Alpha_XYposAtDet_Left2Thirds","H_Alpha_XYposAtDet_Left2Thirds; X [cm]; Y [cm];#alpha_{photon-PMT} [deg];Yield",151, -150., 0.,  251, 50,250, 180,0.,180.);
  H_Alpha_XYposAtDet_RightThird= new TH3D("H_Alpha_XYposAtDet_RightThird","H_Alpha_XYposAtDet_RightThird; X [cm]; Y [cm];#alpha_{photon-PMT} [deg];Yield",151, -150., 0.,  251, 50,250, 180,0.,180.);

  //////////////////////////////////////
  H_dFocalPoint_Delta= new TH1D("H_dFocalPoint_Delta","H_dFocalPoint_Delta;#Delta_{f} [mm];Yield",80,-20.,20.);
  H_dFocalPoint_Rho= new TH1D("H_dFocalPoint_Rho","H_dFocalPoint_Rho;#rho_{f} [mm];Yield",150,50.,200.);

  //cout<<" init hist H_acc_mom_el"<<endl;

  //////////////////////////////////////
  // Detector acceptance efficiency vs. (pt,y) and p
  H_acc_mom_el = new TH1D("H_acc_mom_el", "H_acc_mom_el;p [GeV/c];Yield", 49, 0., 12.);
  H_acc_pty_el = new TH2D("H_acc_pty_el", "H_acc_pty_el;Rapidity;P_{t} [GeV/c];Yield",25, 0., 4., 61, 0., 10.);
  H_Mom_XY_Theta25 = new TH3D("H_Mom_XY_Theta25", "H_Mom_XY_Theta25",151, -150, 0,351, 0, 350, 121, 0., 12.);//25, 0., 4., 61, 0., 10.);
  //////////////////////////////////

  H_NofHitsAll = new TH1D("H_NofHitsAll", "H_NofHitsAll;Nof hits in ring;Yield", 50, 0., 50.);
  H_NofRings = new TH1D("H_NofRings", "H_NofRings;Nof rings per event;Yield", 10, 0., 10.);
  H_NofRings_NofHits = new TH2D("H_NofRings_NofHits", "H_NofRings_NofHits;Nof rings per event, Nof hits per ring;Yield", 10, 0., 10., 50, 0., 50.);

  /////////////////////////////////////////////
  H_Radius= new TH1D("H_Radius","H_Radius",401, 2.,6.);
  H_aAxis= new TH1D("H_aAxis","H_aAxis",401, 2.,10.);
  H_bAxis= new TH1D("H_bAxis","H_bAxis",401, 2.,10.);
  H_boa= new TH1D("H_boa","H_boa",51, 0.5,1.);
  H_boa_RegularTheta= new TH1D("H_boa_RegularTheta","H_boa_RegularTheta",51, 0.5,1.);
  H_boa_LeftHalf= new TH1D("H_boa_LeftHalf","H_boa_LeftHalf",51, 0.5,1.);
  H_boa_RightHalf= new TH1D("H_boa_RightHalf","H_boa_RightHalf",51, 0.5,1.);
  H_boa_Left2Thirds= new TH1D("H_boa_Left2Thirds","H_boa_Left2Thirds",51, 0.5,1.);
  H_boa_RightThird= new TH1D("H_boa_RightThird","H_boa_RightThird",51, 0.5,1.);
  

  H_dR_aa= new TH1D("H_dR_aa","H_dR_aa",61, -3.,3.);  
  H_dR= new TH1D("H_dR","H_dR",61, -3.,3.);  
  H_dR_RegularTheta= new TH1D("H_dR_RegularTheta","H_dR_RegularTheta",61, -3.,3.);  
  H_dR_LeftHalf= new TH1D("H_dR_LeftHalf","H_dR_LeftHalf",61, -3.,3.);  
  H_dR_RightHalf= new TH1D("H_dR_RightHalf","H_dR_RightHalf",61, -3.,3.);  
  H_dR_Left2Thirds= new TH1D("H_dR_Left2Thirds","H_dR_Left2Thirds",61, -3.,3.);  
  H_dR_RightThird= new TH1D("H_dR_RightThird","H_dR_RightThird",61, -3.,3.);  

  //cout<<" init hist H_RingCenter"<<endl;

  H_RingCenter= new TH2D("H_RingCenter","H_RingCenter",201, -100., 0.,351, 0.,350.);
  
  H_RingCenter_Aaxis= new TH3D("H_RingCenter_Aaxis","H_RingCenter_Aaxis",151, -150, 0,351, 0, 350, 80, 2.,10.);
  H_RingCenter_Baxis= new TH3D("H_RingCenter_Baxis","H_RingCenter_Baxis",151, -150, 0,351, 0, 350, 80, 2.,10.);
  H_RingCenter_boa= new TH3D("H_RingCenter_boa","H_RingCenter_boa",151, -150, 0,351, 0, 350, 51, 0.5,1.);
  H_RingCenter_boa_RegularTheta= new TH3D("H_RingCenter_boa_RegularTheta","H_RingCenter_boa_RegularTheta",151, -150, 0,351, 0, 350, 51, 0.5,1.);
  H_RingCenter_boa_LeftHalf= new TH3D("H_RingCenter_boa_LeftHalf","H_RingCenter_boa_LeftHalf",151, -150, 0,351, 0, 350, 51, 0.5,1.);
  H_RingCenter_boa_RightHalf= new TH3D("H_RingCenter_boa_RightHalf","H_RingCenter_boa_RightHalf",151, -150, 0,351, 0, 350, 51, 0.5,1.);
  H_RingCenter_boa_Left2Thirds= new TH3D("H_RingCenter_boa_Left2Thirds","H_RingCenter_boa_Left2Thirds",151, -150, 0,351, 0, 350, 51, 0.5,1.);
  H_RingCenter_boa_RightThird= new TH3D("H_RingCenter_boa_RightThird","H_RingCenter_boa_RightThird",151, -150, 0,351, 0, 350, 51, 0.5,1.);

  H_RingCenter_dR= new TH3D("H_RingCenter_dR","H_RingCenter_dR",151, -150, 0,351, 0, 350, 61, -3.,3.);
  H_RingCenter_dR_RegularTheta= new TH3D("H_RingCenter_dR_RegularTheta","H_RingCenter_dR_RegularTheta",151, -150, 0,351, 0, 350, 61, -3.,3.);
  H_RingCenter_dR_LeftHalf= new TH3D("H_RingCenter_dR_LeftHalf","H_RingCenter_dR_LeftHalf",151, -150, 0,351, 0, 350, 61, -3.,3.);
  H_RingCenter_dR_RightHalf= new TH3D("H_RingCenter_dR_RightHalf","H_RingCenter_dR_RightHalf",151, -150, 0,351, 0, 350, 61, -3.,3.);
  H_RingCenter_dR_Left2Thirds= new TH3D("H_RingCenter_dR_Left2Thirds","H_RingCenter_dR_Left2Thirds",151, -150, 0,351, 0, 350, 61, -3.,3.);
  H_RingCenter_dR_RightThird= new TH3D("H_RingCenter_dR_RightThird","H_RingCenter_dR_RightThird",151, -150, 0,351, 0, 350, 61, -3.,3.);
 
  
}
//////////////////////////////////////////////////////////
void CbmRichGeoOpt::WriteHistograms(){
 
  H_Diff_LineRefPMT_MomAtPMT->Write(); 
  H_Theta_TwoVectors->Write(); 
  H_MomRing->Write(); 
  H_MomPrim->Write(); 
  H_PtPrim->Write(); 
  H_MomPt->Write(); 

  H_Mom_Theta_MC->Write();
  H_Pt_Theta_MC->Write();
  H_Mom_Theta_Rec->Write();
  H_Pt_Theta_Rec->Write();
  H_Mom_Theta_Acc->Write();
  H_Pt_Theta_Acc->Write();

  H_Mom_XY_Theta25->Write(); 

  H_MomPrim_RegularTheta->Write();  
  H_acc_mom_el_RegularTheta->Write(); 
  H_Hits_XY->Write(); 
  H_Hits_XY_LeftHalf->Write(); 
  H_Hits_XY_RightHalf->Write(); 
  H_Hits_XY_RightThird->Write(); 
  H_Hits_XY_Left2Thirds->Write(); 

  H_PointsIn_XY->Write();
  H_PointsIn_XY_LeftHalf->Write();
  H_PointsIn_XY_RightHalf->Write();
  H_PointsIn_XY_RightThird->Write();
  H_PointsIn_XY_Left2Thirds->Write();

  H_PointsOut_XY->Write();
  H_NofPhotonsPerEv->Write(); 
  H_NofPhotonsPerHit->Write(); 
  H_NofPhotonsSmallerThan30->Write(); 
  H_DiffXhit->Write();
  H_DiffYhit->Write();
 
  H_Alpha->Write();
  H_Alpha_UpLeft->Write();
  H_Alpha_UpLeft_II->Write();
  H_Alpha_UpLeft_RegularTheta->Write();
  H_Alpha_UpLeft_LeftHalf->Write();
  H_Alpha_UpLeft_RightHalf->Write();
  H_Alpha_UpLeft_Left2Thirds->Write();
  H_Alpha_UpLeft_RightThird->Write();

  H_Alpha_XYposAtDet->Write();
  H_Alpha_XYposAtDet_RegularTheta->Write();
  H_Alpha_XYposAtDet_LeftHalf->Write();
  H_Alpha_XYposAtDet_RightHalf->Write();
  H_Alpha_XYposAtDet_Left2Thirds->Write();
  H_Alpha_XYposAtDet_RightThird->Write();


  H_acc_mom_el->Write();
  H_acc_pty_el->Write();
  
  H_dFocalPoint_Delta->Write(); 
  H_dFocalPoint_Rho->Write();
  H_NofHitsAll->Write();
  H_NofRings->Write();
  H_NofRings_NofHits->Write();
  H_Radius->Write();
  H_aAxis->Write();
  H_bAxis->Write();
  
  H_boa->Write();
  H_boa_RegularTheta->Write();
  H_boa_LeftHalf->Write(); 
  H_boa_RightHalf->Write();
  H_boa_Left2Thirds->Write(); 
  H_boa_RightThird->Write();

  H_dR_aa->Write();
  H_dR->Write();
  H_dR_RegularTheta->Write();
  H_dR_LeftHalf->Write(); 
  H_dR_RightHalf->Write();
  H_dR_Left2Thirds->Write(); 
  H_dR_RightThird->Write();

  H_RingCenter->Write();
  H_RingCenter_Aaxis->Write();
  H_RingCenter_Baxis->Write();
  
  H_RingCenter_boa->Write();
  H_RingCenter_boa_RegularTheta->Write();
  H_RingCenter_boa_LeftHalf->Write();
  H_RingCenter_boa_RightHalf->Write();
  H_RingCenter_boa_Left2Thirds->Write();
  H_RingCenter_boa_RightThird->Write();
  
  H_RingCenter_dR->Write();
  H_RingCenter_dR_RegularTheta->Write();
  H_RingCenter_dR_LeftHalf->Write();
  H_RingCenter_dR_RightHalf->Write();
  H_RingCenter_dR_Left2Thirds->Write();
  H_RingCenter_dR_RightThird->Write();
}
//////////////////////////////////////////////////////////////
///////////////////////////////
CbmRichPoint* CbmRichGeoOpt::GetPMTPoint(int TrackIdOfSensPlane)
{
  Int_t nofPoints = fRichPoints->GetEntriesFast();
  for(Int_t ip = 0; ip < nofPoints; ip++){
      CbmRichPoint* point = (CbmRichPoint*) fRichPoints->At(ip);
      if(NULL == point) continue;
      int trackId = point->GetTrackID(); if(trackId<0) continue;
      if(trackId == TrackIdOfSensPlane){//cout<<"In Function: got corresponding trackid:"<<trackId<<endl;
	return point;}
  }
  return NULL;
}
//////////////////////////////////////////////////////////////
void CbmRichGeoOpt::FillPointsAtPMT()
{
  
  for(unsigned int p=0;p<PMTPlanePoints.size();p++){
    if(PMTPlanePoints[p].X() != -1000.){
      if(p==0){continue;}
      else{
	int PointFilled=1;
	for(int p2=p-1;p2>-1;p2--){
	  if(TMath::Abs( PMTPlanePoints[p2].X() - PMTPlanePoints[p].X() ) < 1.0){PointFilled=0;}
	}
	if(PointFilled==1){continue;}
      }
    }
    Int_t nofPoints = fRichPoints->GetEntriesFast();
    //  cout<<"We have "<<nofPoints<<" points registered on PMT plane"<<endl;
    for(Int_t ip = 0; ip < nofPoints-10; ip+=10){
      CbmRichPoint* point = (CbmRichPoint*) fRichPoints->At(ip);
      if(NULL == point) continue;
      int trackId = point->GetTrackID(); if(trackId<0) continue;
      if(point->GetX()>=0 || point->GetY()<=0){continue;}
      cout<<"FillPointsAtPMT: Fillinf point #"<<p<<" --> "<<  point->GetX()<<", "<<point->GetY()<<", "<<point->GetZ()<<endl;
      PMTPlanePoints[p].SetX(point->GetX());PMTPlanePoints[p].SetY(point->GetY());PMTPlanePoints[p].SetZ(point->GetZ());
      if(PMTPlanePoints[p].X() !=-1000.){break;}
    }
  }

}
//////////////////////////////////////////////////////////////
void CbmRichGeoOpt::FillPointsAtPMTSensPlane()
{
  
  for(unsigned int p=0;p<SensPlanePoints.size();p++){
    if(SensPlanePoints[p].X() != -1000.){
      if(p==0){continue;}
      else{
	int PointFilled=1;
	for(int p2=p-1;p2>-1;p2--){
	  if(TMath::Abs( SensPlanePoints[p2].X() - SensPlanePoints[p].X() ) < 1.0){PointFilled=0;}
	}
	if(PointFilled==1){continue;}
      }
    }
    
    Int_t nofRefPoints = fRefPoints->GetEntriesFast();
    // cout<<"We have "<<nofRefPoints<<" points registered on sens plane"<<endl;
    for(Int_t ip = 0; ip < nofRefPoints-10; ip+=10){
      CbmRichPoint* RefPoint = (CbmRichPoint*)fRefPoints->At(ip);
     if (RefPoint == NULL) continue;
     int RefPointTrackId = RefPoint->GetTrackID(); if(RefPointTrackId<0) {continue;}
     if(RefPoint->GetX()>=0 || RefPoint->GetY()<=0){continue;}
     
     SensPlanePoints[p].SetX(RefPoint->GetX());SensPlanePoints[p].SetY(RefPoint->GetY());SensPlanePoints[p].SetZ(RefPoint->GetZ());
     if(SensPlanePoints[p].X() !=-1000.){break;}
    }
  }
}

//////////////////////////////////////////////////////


//////////////////////////////////////////////////////
float  CbmRichGeoOpt::GetIntersectionPointsLS( TVector3 MirrCenter,  TVector3 G_P1,  TVector3 G_P2, float R){
  float A=(G_P1-MirrCenter)*(G_P1-MirrCenter);
  float B=(G_P2-G_P1)*(G_P2-G_P1);
  float P=2.*( (G_P1-MirrCenter)*(G_P2-G_P1) )/(B);
  float q=(A-R*R)/B;

  float t1=-1.*P/2.-TMath::Sqrt( (P/2.)*(P/2.) -q);
  float t2=-1.*P/2.+TMath::Sqrt( (P/2.)*(P/2.) -q);
  //cout<<"t1="<<t1<<",  t2="<<t2<<endl;
  //Check if nan --> no intersection
  if(! (t1==1. || t1 >1.) ){return -1.;}
  //cout<<"t1="<<t1<<",  t2="<<t2<<endl;

  TVector3 IntersectP1;  TVector3 IntersectP2;
  IntersectP1.SetX( G_P1.X()+t1*(G_P2.X()-G_P1.X()) );
  IntersectP1.SetY( G_P1.Y()+t1*(G_P2.Y()-G_P1.Y()) );
  IntersectP1.SetZ( G_P1.Z()+t1*(G_P2.Z()-G_P1.Z()) );

  IntersectP2.SetX( G_P1.X()+t2*(G_P2.X()-G_P1.X()) );
  IntersectP2.SetY( G_P1.Y()+t2*(G_P2.Y()-G_P1.Y()) );
  IntersectP2.SetZ( G_P1.Z()+t2*(G_P2.Z()-G_P1.Z()) );

  TVector3 Line1=IntersectP1-G_P1;
  float Length1=TMath::Sqrt(Line1.X()*Line1.X() + Line1.Y()*Line1.Y() + Line1.Z()*Line1.Z());
  TVector3 Line2=IntersectP2-G_P1;
  float Length2=TMath::Sqrt(Line2.X()*Line2.X() + Line2.Y()*Line2.Y() + Line2.Z()*Line2.Z());
  
  //return Length1<Length2 ?  Length1 :  Length2;
  if(Length1<Length2){return Length1;}else{return Length2;}
 }
//////////////////////////////////////////////////////
float  CbmRichGeoOpt::GetDistanceMirrorCenterToPMTPoint(TVector3 PMTpoint)
{
  float XTerm= (PMTpoint.X()-MirrorCenter.X())*(PMTpoint.X()-MirrorCenter.X());
  float YTerm= (PMTpoint.Y()-MirrorCenter.Y())*(PMTpoint.Y()-MirrorCenter.Y());
  float ZTerm= (PMTpoint.Z()-MirrorCenter.Z())*(PMTpoint.Z()-MirrorCenter.Z());
  return TMath::Sqrt(XTerm+YTerm+ZTerm);

}
//////////////////////////////////////////////////////
bool  CbmRichGeoOpt::CheckPointLiesOnPlane(TVector3 Point,TVector3 p0,TVector3 norm )
{
  double TolaratedDiff=0.001;
  double ProdP0WithNorm=p0.Dot(norm); //cout<<"ProdP0WithNorm = "<<ProdP0WithNorm;
  double ProdPWithNorm=Point.Dot(norm); //cout<<"  ProdPWithNorm = "<<ProdPWithNorm<<endl;
  return TMath::Abs(ProdP0WithNorm - ProdPWithNorm) <= ( (TMath::Abs(ProdP0WithNorm) < TMath::Abs(ProdPWithNorm) ? TMath::Abs(ProdPWithNorm) : TMath::Abs(ProdP0WithNorm)) * TolaratedDiff);
}
//////////////////////////////////////////////////////
void CbmRichGeoOpt::GetPMTRotAngels()
{
	CbmRichRecGeoPar* gp = CbmRichGeoManager::GetInstance().fGP;
  RotX=gp->fPmt.fTheta; RotY=gp->fPmt.fPhi;
}

//////////////////////////////////////////////////////
//void  CbmRichGeoOpt::GetPlaneCenter(float rotMir, float rotX, float rotY)
//{
//  PMTPlaneCenterX=MinX+(MaxX-MinX)/2.; PMTPlaneCenterY=MinY+(MaxY-MinY)/2.;
//}

//////////////////////////////////////////////////////
bool  CbmRichGeoOpt::CheckPointLiesOnSphere(TVector3 /*Point*/)
{
  return true;

}

//////////////////////////////////////////////////////
bool  CbmRichGeoOpt::CheckLineIntersectsPlane(TVector3 /*Point*/)
{
	return true;

}
//////////////////////////////////////////////////////
bool  CbmRichGeoOpt::CheckLineIntersectsSphere(TVector3 /*Point*/)
{
	return true;

}
//////////////////////////////////////////////////////
void CbmRichGeoOpt::Finish()
{
  // cout<<nPhotonsNotOnPlane<<" out of "<<nTotalPhorons<<" are not on the plane("<<float(nPhotonsNotOnPlane)/float(nTotalPhorons)<<")"<<endl;
  // cout<<nPhotonsNotOnSphere<<" out of "<<nTotalPhorons<<" are not on the ideal sphere("<<float(nPhotonsNotOnSphere)/float(nTotalPhorons)<<")"<<endl;
  WriteHistograms();
}



ClassImp(CbmRichGeoOpt)
