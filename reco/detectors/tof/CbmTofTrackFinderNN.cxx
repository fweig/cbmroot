// ROOT Classes and includes
#include "TClonesArray.h"
#include <TMath.h>
#include <TGraph2D.h>
#include <TGraph2DErrors.h> 
#include <TRandom2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TF2.h>
#include <TH1.h>
#include "TDirectory.h"
#include "TROOT.h"
#include "TGeoManager.h"

// FAIR classes and includes
#include "FairEventManager.h"           // for FairEventManager
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"
#include "TEveManager.h"                // for TEveManager, gEve

// CBMroot classes and includes
#include "CbmTofFindTracks.h"
#include "CbmTofTrackFinderNN.h"
#include "CbmTofTrackFitter.h"
#include "CbmTofTracklet.h"
#include "CbmTofTrackletParam.h"
#include "CbmTofHit.h"        // in cbmdata/tof
#include "CbmTofGeoHandler.h" // in tof/TofTools
#include "CbmTofDetectorId_v12b.h" // in cbmdata/tof
#include "CbmTofDetectorId_v14a.h" // in cbmdata/tof
#include "CbmTofCell.h"       // in tof/TofData
#include "CbmTofDigiPar.h"    // in tof/TofParam
#include "CbmTofDigiBdfPar.h" // in tof/TofParam
#include "CbmTofAddress.h"    // in cbmdata/tof
#include "CbmMatch.h"
#include "LKFMinuit.h"
#include <CbmEvDisTracks.h>    // in eventdisplay/tof

// C++ includes
#include <vector>
#include <map>
using std::map;
using std::cout;
using std::endl;
   
const Int_t DetMask = 0x3FFFFF; // check for consistency with geometry
LKFMinuit CbmTofTrackFinderNN::fMinuit;

CbmTofTrackFinderNN::CbmTofTrackFinderNN() :
  fHits(NULL),
  fOutTracks(NULL),
  fiNtrks(0),
  fFitter(NULL),
  fFindTracks(NULL),
  fDigiPar(NULL),
  fMaxTofTimeDifference(0.),
  fTxLIM(0.),
  fTyLIM(0.),
  fTxMean(0.),
  fTyMean(0.),
  fSIGLIM(4.),
  fChiMaxAccept(3.),
  fPosYMaxScal(0.55),
  fTracks(),
  fvTrkVec()
{
}

CbmTofTrackFinderNN::~CbmTofTrackFinderNN()
{
}

//Copy constructor
CbmTofTrackFinderNN::CbmTofTrackFinderNN(const CbmTofTrackFinderNN &finder) :
  fHits(NULL),
  fOutTracks(NULL),
  fiNtrks(0),
  fFitter(NULL),
  fFindTracks(NULL),
  fDigiPar(NULL),
  fMaxTofTimeDifference(0.),
  fTxLIM(0.),
  fTyLIM(0.),
  fTxMean(0.),
  fTyMean(0.),
  fSIGLIM(4.),
  fChiMaxAccept(3.),
  fPosYMaxScal(0.55),
  fTracks(),
  fvTrkVec()
{
  // action 
  fHits=finder.fHits;
  fTracks=finder.fTracks;
  fiNtrks=finder.fiNtrks;
}

// assignement operator 
CbmTofTrackFinderNN& CbmTofTrackFinderNN::operator=(const CbmTofTrackFinderNN &/*fSource*/){
  // do copy
  // ... (too lazy) ...
  // return the existing object
  return *this;
}

void CbmTofTrackFinderNN::Init()
{
  /*
  CbmTofFindTracks* fFindTracks = CbmTofFindTracks::Instance();
  fFitter = fFindTracks->GetFitter();
  */
   FairRunAna* ana = FairRunAna::Instance();
   FairRuntimeDb* rtdb=ana->GetRuntimeDb();
   fDigiPar = (CbmTofDigiPar*) (rtdb->getContainer("CbmTofDigiPar"));
   if( NULL == fDigiPar )
   {
      LOG(error)<<"CbmTofTrackFinderNN::Init => Could not obtain the CbmTofDigiPar ";
   }

   LOG(info) << Form(" CbmTofTrackFinderNN::Init : Fitter at 0x%p",fFitter);

  fFindTracks = CbmTofFindTracks::Instance();
  if (NULL == fFindTracks) 
  LOG(fatal) << Form(" CbmTofTrackFinderNN::Init : no FindTracks instance found");

  fMinuit.Initialize();

  LOG(info) << "MaxTofTimeDifference = "<<fMaxTofTimeDifference;
  
}

Int_t CbmTofTrackFinderNN::DoFind(
      TClonesArray* fTofHits,
      TClonesArray* fTofTracks)
{
  fiNtrks=0; // initialize
  fHits   = fTofHits;
  fOutTracks = fTofTracks; //new TClonesArray("CbmTofTracklet");
  //fTracks = new TClonesArray("CbmTofTracklet");
  //if (0 == fFindTracks->GetStationType(0)){ // Generate Pseudo TofHit at origin
  if (0) { //  == fFindTracks->GetAddrOfStation(0)) {  // generate new track seed, disabled
    fFindTracks->SetStation(0,0,0,0);
    const Int_t iDetId = CbmTofAddress::GetUniqueAddress(0,0,0,0,0);
    const TVector3 hitPos(0.,0.,0.);
    const TVector3 hitPosErr(0.5, 0.5, 0.5); // initialize fake hit error
    const Double_t dTime0 = 0.;  // FIXME

    Int_t iNbHits = fHits->GetEntries(); 
    /*CbmTofHit *pHit0 =*/  new((*fHits)[iNbHits]) CbmTofHit( 
			iDetId,
			hitPos, 
			hitPosErr, // local detector coordinates
			iNbHits,   // this number is used as reference!!
                        dTime0,    // Time of hit
       		        0, //vPtsRef.size(), // flag  = number of TofPoints generating the cluster
			0) ;
    LOG(debug1) << "CbmTofTrackFinderNN::DoFind: Fake Hit at origin added at position "<<iNbHits
		<<Form(", DetId 0x%08x",iDetId) 
 	   	; 

  }

  //  fvTrkMap.resize(fHits->GetEntries());
  fvTrkVec.resize(fHits->GetEntries());
  LOG(debug2) <<"<I> TrkMap/Vec resized for "<< fHits->GetEntries() << " entries "; 
  //  for (Int_t iHit=0; iHit<fHits->GetEntries(); iHit++) { fvTrkMap[iHit].clear();}
  for (Int_t iHit=0; iHit<fHits->GetEntries(); iHit++) { fvTrkVec[iHit].clear();}

  LOG(debug) << "MaxTofTimeDifference = "<<fMaxTofTimeDifference;
  Int_t iNTrks=0;
  Int_t iSt0=-1;
  Int_t iSt1=0;
  while(iSt0 < fFindTracks->GetNofStations()-fFindTracks->GetMinNofHits()){        // seed loop, all combinations as seeds 
   iSt0++; iSt1=iSt0;
   while(iSt1 < fFindTracks->GetNofStations()-fFindTracks->GetMinNofHits()+1){
    iSt1++;
    for (Int_t iHit=0; iHit<fHits->GetEntries(); iHit++) { // loop over Hits 
     CbmTofHit* pHit = (CbmTofHit*) fHits->At( iHit );
     Int_t iAddr   = (pHit->GetAddress() & DetMask );
     // Int_t iSmType = CbmTofAddress::GetSmType( iAddr );   (VF) not used
     if(HitUsed(iHit)==1 && iAddr!=fFindTracks->GetBeamCounter()) continue; // skip used Hits except for BeamCounter
     LOG(debug1) << Form("<I> TofTracklet Chkseed St0 %2d, St1 %2d, Mul %2d, Hit %2d, addr = 0x%08x - X %6.2f, Y %6.2f Z %6.2f R %6.2f T %6.2f TM %lu",
			 iSt0,iSt1,fiNtrks,iHit,pHit->GetAddress(),pHit->GetX(),pHit->GetY(),pHit->GetZ(),pHit->GetR(),pHit->GetTime(), fvTrkVec[iHit].size() ); 
     if (iAddr == fFindTracks->GetAddrOfStation(iSt0)) {  // generate new track seed
     LOG(debug) << Form("<I> TofTracklet seed St0 %2d, St1 %2d, Mul %2d, Hit %2d, addr = 0x%08x - X %6.2f, Y %6.2f Z %6.2f T %6.2f TM %lu",
		       iSt0,iSt1,fiNtrks,iHit,pHit->GetAddress(),pHit->GetX(),pHit->GetY(),pHit->GetZ(),pHit->GetTime(), fvTrkVec[iHit].size() ); 
 
      Int_t iChId = pHit->GetAddress();
      CbmTofCell* fChannelInfo = fDigiPar->GetCell( iChId );
      Int_t iCh = CbmTofAddress::GetChannelId(iChId);
      Double_t hitpos[3]={3*0.};
      Double_t hitpos_local[3]={3*0.};
      Double_t dSizey=1.;

      if(1) { // iSmType>0) { // prevent geometry inspection for FAKE hits 
      if(NULL == fChannelInfo){
	LOG(fatal) <<"<D> CbmTofTrackFinderNN::DoFind0: Invalid Channel Pointer for ChId "
		   << Form(" 0x%08x ",iChId)<<", Ch "<<iCh;
	//	continue;
      }else{
	/*TGeoNode *fNode= */       // prepare global->local trafo
	gGeoManager->FindNode(fChannelInfo->GetX(),fChannelInfo->GetY(),fChannelInfo->GetZ());
	if( fFindTracks->GetAddrOfStation(iSt0)  !=  fFindTracks->GetBeamCounter() ) {
	  hitpos[0]=pHit->GetX();
	  hitpos[1]=pHit->GetY();
	  hitpos[2]=pHit->GetZ();
	  /*TGeoNode* cNode=*/ gGeoManager->GetCurrentNode();
	  gGeoManager->MasterToLocal(hitpos, hitpos_local);
	}
	dSizey=fChannelInfo->GetSizey();
	LOG(debug) << Form("<D> TofTracklet start %d, Hit %d - yloc %6.2f, dy %6.2f, Scal %6.2f -> stations 0x%08x, 0x%08x,",
			   fiNtrks,iHit,hitpos_local[1],dSizey,fPosYMaxScal,fFindTracks->GetAddrOfStation(iSt0),fFindTracks->GetAddrOfStation(iSt1)  ) ; 
      }
      }

      if(TMath::Abs(hitpos_local[1])<dSizey*fPosYMaxScal)
      for (Int_t iHit1=0; iHit1<fHits->GetEntries(); iHit1++) // loop over all Hits (order unknown) 
      {
	if(HitUsed(iHit1)==1) continue;                       // skip used Hits
	CbmTofHit* pHit1 = (CbmTofHit*) fHits->At( iHit1 );
	// Int_t iSmType1 = CbmTofAddress::GetSmType( pHit1->GetAddress() & DetMask );   (VF) not used
	Int_t iAddr1 = ( pHit1->GetAddress() & DetMask );
        //if (iSmType1 == fFindTracks->GetStationType(1)) { // generate new track seed
	if (iAddr1 == fFindTracks->GetAddrOfStation(iSt1)) {  // generate new track seed
	  Int_t iChId1 = pHit1->GetAddress();
	  CbmTofCell* fChannelInfo1 = fDigiPar->GetCell( iChId1 );
	  Int_t iCh1 = CbmTofAddress::GetChannelId(iChId1);	
	  Double_t hitpos1[3]={3*0.};
	  Double_t hitpos1_local[3]={3*0.};
	  Double_t dSizey1=1.;
	  if(NULL == fChannelInfo1){
	    LOG(debug) << "CbmTofTrackFinderNN::DoFindi: Invalid Channel Pointer for ChId "
		       << Form(" 0x%08x ",iChId1)<<", Ch "<<iCh1 ;
	    //	    continue;
	  }else{
	    /*TGeoNode *fNode1=*/        // prepare global->local trafo
	    gGeoManager->FindNode(fChannelInfo1->GetX(),fChannelInfo1->GetY(),fChannelInfo1->GetZ());
	    hitpos1[0]=pHit1->GetX();
	    hitpos1[1]=pHit1->GetY();
	    hitpos1[2]=pHit1->GetZ();
	    /*TGeoNode* cNode1=*/ gGeoManager->GetCurrentNode();
	    gGeoManager->MasterToLocal(hitpos1, hitpos1_local);
	    dSizey1=fChannelInfo1->GetSizey();
	  }
	  Double_t dDT = pHit1->GetTime() - pHit->GetTime();
	  //if(dDT<0.) continue;  // request forward propagation in time  

	  Double_t dLz =  pHit1->GetZ()   - pHit->GetZ();
	  Double_t dTx = (pHit1->GetX()   - pHit->GetX())/dLz;
	  Double_t dTy = (pHit1->GetY()   - pHit->GetY())/dLz;
	  Double_t dDist = TMath::Sqrt( TMath::Power((pHit->GetX()-pHit1->GetX()),2)
				      + TMath::Power((pHit->GetY()-pHit1->GetY()),2)
				      + TMath::Power((pHit->GetZ()-pHit1->GetZ()),2)
					); 
	  LOG(debug1) << Form("<I> TofTracklet %d, Hits %d, %d, add = 0x%08x,0x%08x - DT %6.2f, Tx %6.2f Ty %6.2f Tt %6.2f pos %6.2f %6.2f %6.2f ",
			      fiNtrks,iHit,iHit1,pHit->GetAddress(),pHit1->GetAddress(), dDT, dTx, dTy, dDT/dLz, hitpos1_local[0],hitpos1_local[1],hitpos1_local[2] ) ; 
          LOG(debug3) << Form("    selection: y %6.2f < %6.2f, T %6.2f < %6.2f, dTpos %6.2f < %6.2f, Abs(%6.2f - %6.2f) < %6.2f",
			      hitpos1_local[1],dSizey1*fPosYMaxScal,dDT/dLz,fMaxTofTimeDifference,dTx,fTxLIM,dTy,fTyMean,fTyLIM) ; 

	  if(    TMath::Abs(hitpos1_local[1])<dSizey1*fPosYMaxScal)
	    if(  TMath::Abs(dDT/dLz)<fMaxTofTimeDifference && TMath::Abs(dDT/dDist)>0.005 // FIXME: numeric constant in code 
	      && TMath::Abs(dTx-fTxMean)<fTxLIM &&  TMath::Abs(dTy-fTyMean)<fTyLIM)
	  {
	    CbmTofTracklet* pTrk = new CbmTofTracklet();
	    fTracks.push_back(pTrk);
	    pTrk->SetTofHitIndex(iHit,iAddr,pHit);        // store 1. Hit index
	    pTrk->AddTofHitIndex(iHit1,iAddr1,pHit1); // store 2. Hit index
	    fiNtrks=fTracks.size();

	    fvTrkVec[iHit].push_back(pTrk);
	    fvTrkVec[iHit1].push_back(pTrk);

	    pTrk->SetTime(pHit->GetTime());          // define reference time from 1. plane   
	    //Double_t dR  = pHit1->GetR() - pHit->GetR();
	    Double_t dR  = pTrk->Dist3D(pHit1,pHit);
	    Double_t dTt = fFindTracks->GetTtTarg(); // assume calibration target value 
	    if( 0 ) { //== iSmType) {  // disabled
	      Double_t T0Fake = pHit->GetTime();
	      Double_t w=fvTrkVec[iHit].size();
	      T0Fake=(T0Fake*(w-1.)+(pHit1->GetTime() - dTt * dR))/w;
	      LOG(debug1) << Form("<I> TofTracklet %d, Fake T0, old %8.0f -> new %8.0f",fiNtrks, pHit->GetTime(), T0Fake)
			  ; 
	      pHit->SetTime(T0Fake);
	    }
	    Double_t dSign=1.;
	    if(pHit1->GetZ() < pHit->GetZ()) dSign=-1.; 
	    dTt = dSign * dDT / dR;
	    pTrk->SetTt(dTt);                        // store inverse velocity    
	    pTrk->UpdateT0();
	    CbmTofTrackletParam *tPar = pTrk->GetTrackParameter();
	    tPar->SetX( pHit->GetX() ); // fill TrackParam
	    tPar->SetY( pHit->GetY() ); // fill TrackParam
	    tPar->SetZ( pHit->GetZ() ); // fill TrackParam
	    tPar->SetLz(dLz);
	    tPar->SetTx(dTx);
	    tPar->SetTy(dTy);

	    LOG(debug) << Form("<I> TofTracklet %d, Hits %d, %d initialized, add 0x%08x,0x%08x, DT %6.3f, Sgn %2.0f, DR %6.3f, T0 %6.2f, Tt %6.4f ",
			       fiNtrks,iHit,iHit1,pHit->GetAddress(), pHit1->GetAddress(), 
			       dDT, dSign, dR, pTrk->GetT0(), pTrk->GetTt())
	      //		      << tPar->ToString()
	   	      ; 
	  }
	}
      }
     }   // iSt0 condition end  
    }   // Loop on Hits end    

    if(iNTrks >= 0 && static_cast<size_t>(iNTrks) == fTracks.size()) continue; // nothing new
    iNTrks=fTracks.size();
    PrintStatus((char*)Form("after seeds of St0 %d, St1 %d, Mul %d",iSt0,iSt1,iNTrks));

    const Int_t MAXNCAND=1000;  // Max number of hits matchable to current tracklets
    // Propagate track seeds to remaining detectors
    for(Int_t iDet=iSt1+1; iDet<fFindTracks->GetNStations(); iDet++) { 
      Int_t iNCand=0;
      Int_t iHitInd[MAXNCAND];
      CbmTofTracklet*  pTrkInd[MAXNCAND];
      Double_t dChi2[MAXNCAND];
      for (size_t iTrk=0; iTrk<fTracks.size(); iTrk++) { // loop over Trackseeds
	CbmTofTracklet* pTrk = (CbmTofTracklet *)fTracks[iTrk];
	LOG(debug3)<<"     Propagate Loop "<<iTrk<<" pTrk "<<pTrk
		   <<Form(" to station %d, addr: 0x%08x ",iDet,fFindTracks->GetAddrOfStation(iDet));
	if(NULL == pTrk) continue;

	for (Int_t iHit=0; iHit<fHits->GetEntries(); iHit++) { // loop over Hits 
	  if(HitUsed(iHit)==1) continue;                       // skip used Hits
	  CbmTofHit* pHit = (CbmTofHit*) fHits->At( iHit );
	  Int_t iAddr = ( pHit->GetAddress() & DetMask );
	  if (iAddr != fFindTracks->GetAddrOfStation(iDet)) continue;
 
	  // Int_t iSmType = CbmTofAddress::GetSmType( pHit->GetAddress() & DetMask );  (VF) not used
	  Int_t iChId = pHit->GetAddress();
	  CbmTofCell* fChannelInfo = fDigiPar->GetCell( iChId );
	  Int_t iCh = CbmTofAddress::GetChannelId(iChId);
	  Double_t hitpos[3]={3*0.};
	  Double_t hitpos_local[3]={3*0.};
	  Double_t dSizey=1.;

	  if(NULL == fChannelInfo){
	    LOG(debug) << "CbmTofTrackFinderNN::DoFind: Invalid Channel Pointer from Hit "<<iHit<<" for ChId "
		       << Form(" 0x%08x ",iChId)<<", Ch "<<iCh;
	    //	continue;
	  }else{
	    /*TGeoNode *fNode=*/        // prepare global->local trafo
	    gGeoManager->FindNode(fChannelInfo->GetX(),fChannelInfo->GetY(),fChannelInfo->GetZ());
	    hitpos[0]=pHit->GetX();
	    hitpos[1]=pHit->GetY();
	    hitpos[2]=pHit->GetZ();
	    /*TGeoNode* cNode=*/ gGeoManager->GetCurrentNode();
	    gGeoManager->MasterToLocal(hitpos, hitpos_local);
	    dSizey=fChannelInfo->GetSizey();
	  }
	  if(TMath::Abs(hitpos_local[1])<dSizey*fPosYMaxScal)
	  {                                                    // extrapolate Tracklet to this station
	    if(pTrk->GetStationHitIndex(iAddr) > -1) continue; // Station already part of this tracklet
	    CbmTofTrackletParam *tPar = pTrk->GetTrackParameter();
	    Int_t iHit0 = pTrk->GetTofHitIndex(0);
	    Int_t iHit1 = pTrk->GetTofHitIndex(1);
	    // CbmTofHit* pHit0 = (CbmTofHit*) fHits->At( iHit0 );   (VF) not used
	    // CbmTofHit* pHit1 = (CbmTofHit*) fHits->At( iHit1 );   (VF) not used
	    if(iHit0 < 0 || iHit0 >= fHits->GetEntries())
	      LOG(fatal)<<"CbmTofTrackFinderNN::DoFind Invalid Hit Index "<<iHit0
			<<" for Track "<<iTrk<<"("<<fTracks.size()<<")";
	    Double_t dDz  = pHit->GetZ() - tPar->GetZ();
	    Double_t dXex = tPar->GetX() + tPar->GetTx()*dDz;  // pTrk->GetFitX(pHit->GetZ()); 
	    Double_t dYex = tPar->GetY() + tPar->GetTy()*dDz;  // pTrk->GetFitY(pHit->GetZ());  
	    /*
	    Double_t dDr  = pHit->GetR() - pHit0->GetR();
	    Double_t dTex = pHit0->GetTime() + (pHit1->GetTime()-pHit0->GetTime())/(pHit1->GetR()-pHit0->GetR())*dDr;
	    */
	    Double_t dTex = pTrk->GetTex(pHit);
	    // pTrk->GetFitT(pHit->GetR());

	    Double_t dChi = TMath::Sqrt((TMath::Power(TMath::Abs(dTex-pHit->GetTime())/fFindTracks->GetSigT(iAddr),2)
				        +TMath::Power(TMath::Abs(dXex-pHit->GetX())/fFindTracks->GetSigX(iAddr),2)
					+TMath::Power(TMath::Abs(dYex-pHit->GetY())/fFindTracks->GetSigY(iAddr),2))/3);

	    LOG(debug1)<<Form("<IP> TofTracklet %lu, HMul %d, Hits %d, %d check %d, Station 0x%08x: DT %f, DX %f, DY %f, Chi %f",
			    iTrk,pTrk->GetNofHits(),iHit0,iHit1,iHit,iAddr,
			    (dTex-pHit->GetTime())/fFindTracks->GetSigT(iAddr), 
			    (dXex-pHit->GetX())/fFindTracks->GetSigX(iAddr),
			    (dYex-pHit->GetY())/fFindTracks->GetSigY(iAddr), dChi); 

      if(   dChi < fSIGLIM )  // FIXME: should scale limit with material budget between hit and track reference
	    { // extend and update tracklet 
	      LOG(debug) << Form("<IP> TofTracklet %lu, HMul %d, Hits %d, %d mark for extension by %d, add = 0x%08x, DT %6.2f, DX %6.2f, DY=%6.2f ",
				 iTrk,pTrk->GetNofHits(),iHit0,iHit1,iHit, pHit->GetAddress(), dTex-pHit->GetTime(),dXex-pHit->GetX(),dYex-pHit->GetY() )
			 << tPar->ToString(); 

	      if(iNCand>0) {
		LOG(debug)<<Form("CbmTofTrackFinderNN::DoFind new match %d of Hit %d, Trk %lu, chi2 = %f", iNCand,iHit,iTrk,dChi);
		iNCand++;
		if(iNCand==MAXNCAND) iNCand--;  // Limit index to maximum 

		for (Int_t iCand=0; iCand<iNCand; iCand++){
		  if (dChi < dChi2[iCand]){
		    for(Int_t iCC=iNCand; iCC>iCand; iCC--){
		      pTrkInd[iCC]=pTrkInd[iCC-1];
		      iHitInd[iCC]=iHitInd[iCC-1];
		      dChi2[iCC]=dChi2[iCC-1];
		    }
		    pTrkInd[iCand]=pTrk;
		    iHitInd[iCand]=iHit;
		    dChi2[iCand]=dChi;
		    dChi2[iNCand]=1.E8;
		    LOG(debug1)<< Form(" <D> candidate inserted at pos %d",iCand);
		    break;
		  }
		}
	      }else{
		LOG(debug)<<Form("CbmTofTrackFinderNN::DoFind first match %d of Hit %d, Trk %p, chi2 = %f", iNCand,iHit,pTrk,dChi);
		pTrkInd[iNCand]=pTrk;
		iHitInd[iNCand]=iHit;
		dChi2[iNCand]=dChi; // relative quality measure
		iNCand++;
		dChi2[iNCand]=1.E8;
	      } 
	    }

	  } // hit y position check end 
	}   // hit loop end
      }    // loop over tracklets end 

      while(iNCand>0) {  // at least one matching hit - trk pair found
	CbmTofTracklet* pTrk = pTrkInd[0];
	if(NULL == pTrk) continue;
	LOG(debug) << Form("%d hit match candidates in station %d to %lu TofTracklets",iNCand,iDet,fTracks.size());
	for (Int_t iM=0; iM<iNCand; iM++) {
	  pTrk = (CbmTofTracklet *)pTrkInd[iM];
	  if(NULL == pTrk) break;
	  std::vector<CbmTofTracklet *>::iterator it=std::find(fTracks.begin(),fTracks.end(),pTrk);
	  if(it == fTracks.end()) break; // track candidate not existing 

	  LOG(debug1) << "\t" << Form("Hit %d, Trk %p  with chi2 %f (%f)", iHitInd[iM],  pTrkInd[iM], dChi2[iM], pTrk->GetMatChi2(fFindTracks->GetAddrOfStation(iDet)));
	}
        PrintStatus((char*)"starting NCand");

	// check if best pTrk still active
	pTrk = (CbmTofTracklet *)pTrkInd[0];
	size_t iTr=0;
	for(; iTr<fTracks.size(); iTr++){
	  if(fTracks[iTr]==pTrk) {
	    LOG(debug) << "Track "<<pTrk<<" active at pos "<<iTr;
	    break;
	  }
	}
	if(iTr == fTracks.size()) {
	  iNCand--;
	  for(Int_t iCand=0; iCand<iNCand; iCand++){
	    pTrkInd[iCand]=pTrkInd[iCand+1];
	    iHitInd[iCand]=iHitInd[iCand+1];
	    dChi2[iCand]=dChi2[iCand+1];
	  }
	  continue; 
	}

	// CbmTofTrackletParam *tPar = pTrk->GetTrackParameter();   (VF) not used
	Int_t iHit0 = pTrk->GetTofHitIndex(0);
	Int_t iHit1 = pTrk->GetTofHitIndex(1);
	if(pTrk->GetNofHits() > fFindTracks->GetNStations() || pTrk->GetNofHits()<=0) 
	  LOG(fatal)<< " No or more Tracklet hits than stations ! Stop " ;
	// check if tracklet already contains a hit of this layer
	Int_t iHit=iHitInd[0];
	CbmTofHit* pHit = (CbmTofHit*) fHits->At( iHit );
	Int_t iAddr = ( pHit->GetAddress() & DetMask );
	if(Double_t dLastChi2 = pTrk->GetMatChi2(fFindTracks->GetAddrOfStation(iDet)) == -1.){
	  LOG(debug1) <<Form(" -D- Add hit %d at %p, Addr 0x%08x, Chi2 %6.2f to size %u",iHit,pHit,iAddr,dChi2[0],pTrk->GetNofHits());
	  pTrk->AddTofHitIndex(iHit,iAddr,pHit,dChi2[0]); // store next Hit index with matching chi2
	  //	  pTrk->PrintInfo();
	  fvTrkVec[iHit].push_back(pTrk);
	  Line3Dfit(pTrk);                   // full MINUIT fit overwrites ParamLast!
	  Bool_t bkeep=kFALSE;
	  if(pTrk->GetChiSq() > fChiMaxAccept) {
	    LOG(debug) <<Form("Add hit %d invalidates tracklet with Chi %6.2f > %6.2f -> undo ",
			      iHit,pTrk->GetChiSq(),fChiMaxAccept);
	    fvTrkVec[iHit].pop_back();
	    pTrk->RemoveTofHitIndex(iHit,iAddr,pHit,dChi2[0]); 
	    Line3Dfit(pTrk);                //restore old status
	    bkeep=kTRUE;
	  }

	  if(bkeep) {
	    iNCand--;
	    for(Int_t iCand=0; iCand<iNCand; iCand++){
	      pTrkInd[iCand]=pTrkInd[iCand+1];
	      iHitInd[iCand]=iHitInd[iCand+1];
	      dChi2[iCand]=dChi2[iCand+1];
	    }
	  }else {  // update chi2 array
	    PrintStatus((char*)"before UpdateTrackList");
	    UpdateTrackList(pTrk);  
	    Int_t iNCandNew = 0;
	    for(Int_t iCand=0; iCand<iNCand; iCand++){
              if( pTrk !=  pTrkInd[iCand] && iHit != iHitInd[iCand] ) {
		pTrkInd[iNCandNew]=pTrkInd[iCand];
		iHitInd[iNCandNew]=iHitInd[iCand];
		dChi2[iNCandNew]=dChi2[iCand];
		iNCandNew++;
	      }
	    }	    
	    iNCand=iNCandNew;
	  }
	}
	else {
	  if ( dChi2[0]<dLastChi2 ) { // replace hit index
	    LOG(fatal) <<Form("-D- Replace %d, Addr 0x%08x, at %p, Chi2 %6.2f",iHit,iAddr,pHit,dChi2[0]);
	    //cout << " -D- Replace " << endl;
	    pTrk->ReplaceTofHitIndex(iHit,iAddr,pHit,dChi2[0]);
	    // TODO remove tracklet assigment of old hit! FIXME
	  }else{
	    LOG(debug) <<Form("    -D- Ignore %d, Det %d, Addr 0x%08x, at 0x%p, Chi2 %6.2f",iHit,iDet, iAddr,pHit,dChi2[0]);
	    // Form new seeds
	    //if (iDet<(fFindTracks->GetNStations()-1))	TrklSeed(fHits,fTracks,iHit);
	    break;
	  }
	}  

	// pTrk->SetParamLast(tPar);       // Initialize FairTrackParam for KF 
	//fFitter->DoFit(pTrk);            //whatever that means ... KF - Fitting
	//pTrk->GetFairTrackParamLast();   // transfer fit result to CbmTofTracklet
	//pTrk->SetTime(pHit->GetTime());  // update reference time
	  
	//Line3Dfit(pTrk);                   // full MINUIT fit for debugging overwrites ParamLast!
	  
	pTrk->SetTime(pTrk->UpdateT0());   // update reference time (and fake hit time) 

	//FairTrackParam paramExtr;
	//fFitter->Extrapolate(pTrk->GetParamLast(),0.,&paramExtr);
	//pTrk->GetParamFirst()->Print();
	//pTrk->GetParamLast()->Print();
	//paramExtr.Print();

	// check with ROOT fitting method 
	//TLinearFitter *lf=new TLinearFitter(3);
	//lf->SetFormula("hyp3");
 
	// update inverse velocity
	Double_t dTt=pTrk->GetTt();

	LOG(debug)   << Form("<Res> TofTracklet %p, HMul %d, Hits %d, %d, %d, NDF %d,  Chi2 %6.2f, T0 %6.2f, Tt %6.4f ",
			     pTrk,pTrk->GetNofHits(),iHit0,iHit1,iHit, pTrk->GetNDF(), pTrk->GetChiSq(), pTrk->GetTime(), dTt);
	PrintStatus((char*)"<Res> ");
	LOG(debug1) << " Match loop status: NCand " << iNCand << ", iDet " << iDet;

	/* live display insert 
	if(gLogger->IsLogNeeded(debug3))  // update event display, if initialized 
	  {
	     Int_t ii;
	     CbmEvDisTracks* fDis = CbmEvDisTracks::Instance();
	     if(NULL != fDis) {
	       //gEve->Redraw3D(kTRUE);
	       //gEve->FullRedraw3D();
	       fiNtrks=0;
	       for(Int_t iTr=0; iTr<fTracks.size(); iTr++){
		 if(fTracks[iTr]==NULL) continue;
		 CbmTofTracklet* pTrkDis = new((*fTofTracks)[fiNtrks++]) CbmTofTracklet (*fTracks[iTr]);
	       }
	       fDis->Exec("");
	     }
	     cout << " fDis "<<fDis<<" with "<<fiNtrks<<" tracks, to continue type 0 ! "<<endl;
	     scanf("%d",&ii);
	  }  // end of live display 
	*/ 
      }     // end of  while(iNCand>0) 
    }      // detector loop (propagate) end 
   } // iSt1 while condition end
  }   // iSt0 while condition end
  //fTracks->Compress();
  //fTofTracks = fTracks;

  // copy fTracks -> fTofTracks / fOutTracks


  fiNtrks=0;
  for(size_t iTr=0; iTr<fTracks.size(); iTr++){
    if(fTracks[iTr]==NULL) continue;
    if(fTracks[iTr]->GetNofHits() < 3) continue;            // request minimum number of hits (3) 
    if(fTracks[iTr]->GetChiSq() > fChiMaxAccept) continue;  // request minimum ChiSq (3) 
    CbmTofTracklet* pTrk = new((*fTofTracks)[fiNtrks++]) CbmTofTracklet (*fTracks[iTr]);

    if(gLogger->IsLogNeeded( fair::Severity::debug )) {
      LOG(info)<<"Found Trkl "<<iTr<<", ";
      pTrk->PrintInfo();
    }
    for(Int_t iHit=0; iHit<pTrk->GetNofHits(); iHit++){ // mark used Hit
      CbmTofHit* pHit = (CbmTofHit*) fHits->At( pTrk->GetHitIndex(iHit) );
      pHit->SetFlag(pHit->GetFlag()+100.);
      LOG(debug) << Form(" hit %d at %d flagged to %d ",iHit, pTrk->GetHitIndex(iHit), pHit->GetFlag());
    }
  }
  PrintStatus((char*)"<D> Final result");

  for(size_t iTr=0; iTr<fTracks.size(); iTr++){
    if(fTracks[iTr]==NULL) continue;
    fTracks[iTr]->Delete();
    //delete    fTracks[iTr];
    LOG(debug) << Form("<I> TofTracklet %lu, %p deleted",
			    iTr,fTracks[iTr] );
  }
  fTracks.resize(0); //cleanup 
  // fFindTracks->PrintSetup();
  return 0;
}        // DoFind end 

void  CbmTofTrackFinderNN::TrklSeed(Int_t iHit)
{
  CbmTofHit* pHit = (CbmTofHit*) fHits->At( iHit );
  // Int_t iSmType   = CbmTofAddress::GetSmType( pHit->GetAddress() & DetMask );   (VF) not used
  Int_t iAddr     = (pHit->GetAddress() & DetMask );
  //Int_t iDet      = fFindTracks->GetTypeStation(iSmType);
  Int_t iDet      = fFindTracks->GetStationOfAddr(iAddr);
  if(iDet == fFindTracks->GetNofStations()) return;  // hit not in tracking setup
  for(Int_t iDet1=0; iDet1<iDet; iDet1++) {  // build new seeds  
    for (Int_t iHit1=0; iHit1<fHits->GetEntries(); iHit1++) {// loop over previous Hits
      CbmTofHit* pHit1 = (CbmTofHit*) fHits->At( iHit1 );
      // Int_t iSmType1 = CbmTofAddress::GetSmType( pHit1->GetAddress() & DetMask );   (VF) not used
      Int_t iAddr1 = (pHit1->GetAddress() & DetMask );
      if(iAddr1 == iAddr) continue;
      //if (iSmType1 == fFindTracks->GetStationType(iDet1)) {      // generate candidate for new track seed
      if (iAddr1 == fFindTracks->GetAddrOfStation(iDet1)) {      // generate candidate for new track seed
	Int_t iChId1 = pHit1->GetAddress();
	CbmTofCell* fChannelInfo1 = fDigiPar->GetCell( iChId1 );
	Int_t iCh1 = CbmTofAddress::GetChannelId(iChId1);
	Double_t hitpos1[3]={3*0.};
	Double_t hitpos1_local[3]={3*0.};
	Double_t dSizey1=1.;

	if(NULL == fChannelInfo1){
	  LOG(debug) << "CbmTofTrackFinderNN::DoFindp: Invalid Channel Pointer for ChId "
	      << Form(" 0x%08x ",iChId1)<<", Ch "<<iCh1;
	  //  continue;
	}else{
	  /*TGeoNode *fNode1=*/        // prepare global->local trafo
	  gGeoManager->FindNode(fChannelInfo1->GetX(),fChannelInfo1->GetY(),fChannelInfo1->GetZ());
	  hitpos1[0]=pHit1->GetX();
	  hitpos1[1]=pHit1->GetY();
	  hitpos1[2]=pHit1->GetZ();
	  /*TGeoNode* cNode1=*/ gGeoManager->GetCurrentNode();
	  gGeoManager->MasterToLocal(hitpos1, hitpos1_local);
	  dSizey1=fChannelInfo1->GetSizey();
	}
	Double_t dDT = pHit->GetTime()- pHit1->GetTime();
	Double_t dLz = pHit->GetZ()   - pHit1->GetZ();
	Double_t dTx = (pHit->GetX() - pHit1->GetX())/dLz;
	Double_t dTy = (pHit->GetY() - pHit1->GetY())/dLz;
	Int_t iUsed  = HitUsed(iHit1);
	LOG(debug1) << Form("<ISeed> TofTracklet %d, Hits %d, %d, used %d check, add = 0x%08x,0x%08x - DT %6.2f, Tx %6.2f Ty %6.2f ",
			    fiNtrks,iHit,iHit1,iUsed,pHit->GetAddress(),pHit1->GetAddress(), dDT, dTx, dTy ); 
	if(TMath::Abs(hitpos1_local[1])<dSizey1*fPosYMaxScal)
	if(   TMath::Abs(dDT/dLz)<fMaxTofTimeDifference && TMath::Abs(dTx-fTxMean)<fTxLIM 
          &&  TMath::Abs(dTy-fTyMean)<fTyLIM && iUsed==0 )
	{
	  //	  CbmTofTracklet* pTrk = new((*fTracks)[++fiNtrks]) CbmTofTracklet(); // generate new track seed 
	  CbmTofTracklet* pTrk = new CbmTofTracklet();
	  ++fiNtrks;
	  fTracks.push_back(pTrk);
	  pTrk->SetTofHitIndex(iHit1,iAddr1,pHit1); // store Hit index
	  //Int_t NTrks1=fvTrkMap[iHit1].size()+1;
	  //fvTrkMap[iHit1].insert(std::pair<CbmTofTracklet*,Int_t>(pTrk,NTrks1));
	  fvTrkVec[iHit1].push_back(pTrk);

	  pTrk->AddTofHitIndex(iHit,iAddr,pHit);   // store 2. Hit index
	  //Int_t NTrks=fvTrkMap[iHit].size()+1;
	  //fvTrkMap[iHit].insert(std::pair<CbmTofTracklet*,Int_t>(pTrk,NTrks));
	  fvTrkVec[iHit].push_back(pTrk);

          pTrk->SetTime(pHit->GetTime());       // define reference time from 2. plane   
	  Double_t dR  = pHit->GetR() - pHit1->GetR();
	  Double_t dTt = 1./30. ; // assume speed of light:  1 / 30 cm/ns
	  // if( 0 == iSmType) pHit1->SetTime(pHit->GetTime() - dTt * dR);
	  dTt = (pHit->GetTime() - pHit1->GetTime())/dR;
	  pTrk->SetTt(dTt); 
	  pTrk->UpdateT0();
	  
	  CbmTofTrackletParam *tPar = pTrk->GetTrackParameter();
	  tPar->SetX( pHit->GetX() ); // fill TrackParam
	  tPar->SetY( pHit->GetY() ); // fill TrackParam
	  tPar->SetZ( pHit->GetZ() ); // fill TrackParam
	  tPar->SetLz(dLz);
	  tPar->SetTx(dTx);
	  tPar->SetTy(dTy);
	  LOG(debug) << Form("<DSeed> TofTracklet %d, Hits %d, %d add initialized, add = 0x%08x,0x%08x ",
			    fiNtrks,iHit,iHit1,
			    pHit->GetAddress(),pHit1->GetAddress());
	  PrintStatus((char*)"after DSeed");
	}
      }
    } // hit loop end   
  } // Station loop end 
}

Int_t  CbmTofTrackFinderNN::HitUsed(Int_t iHit)
{
  // CbmTofHit* pHit = (CbmTofHit*) fHits->At( iHit );   (VF) not used
  //Int_t iSmType   = CbmTofAddress::GetSmType( pHit->GetAddress() & DetMask );
  //Int_t iDet    = fFindTracks->GetTypeStation(iSmType);
  Int_t iUsed     = 0;

  //  LOG(debug1)<<"CbmTofTrackFinderNN::HitUsed of Hind "<<iHit<<", TrkMap.size "<<fvTrkMap[iHit].size()
  LOG(DEBUG4)<<"CbmTofTrackFinderNN::HitUsed of Hind "<<iHit<<", TrkVec.size "<<fvTrkVec[iHit].size();
  /*
  for ( std::map<CbmTofTracklet*,Int_t>::iterator it=fvTrkMap[iHit].begin(); it != fvTrkMap[iHit].end(); it++){
    if(it->first->GetNofHits() > 2) return iUsed=1;
  }
  */
  if(fvTrkVec[iHit].size()>0) {
    if (fvTrkVec[iHit][0]->GetNofHits()>2) iUsed=1;
  }
  return iUsed;
}

void  CbmTofTrackFinderNN::UpdateTrackList( Int_t iTrk)
{
  CbmTofTracklet* pTrk = (CbmTofTracklet *)fTracks[iTrk];
  UpdateTrackList(pTrk);
}

void  CbmTofTrackFinderNN::UpdateTrackList( CbmTofTracklet* pTrk)
{
  for (Int_t iHit=0; iHit<pTrk->GetNofHits(); iHit++) {    // loop over Tracklet Hits 
    Int_t iHitInd = pTrk->GetHitIndex(iHit);   // Hit index in fHits
    //Int_t NTrks=fvTrkMap[iHitInd].size();    // Number of tracks containing this hit
    Int_t NTrks=fvTrkVec[iHitInd].size();      // Number of tracks containing this hit
    Int_t iAddr   = ( pTrk->GetTofHitPointer(iHit)->GetAddress() & DetMask );
    if(iAddr == fFindTracks->GetBeamCounter()) continue;  // keep all tracklets from common beam reference counter

    // Int_t iSmType = CbmTofAddress::GetSmType( iAddr );   (VF) not used
    //if(iSmType==0) continue;                          // keep all tracklets with common target faked hit

    if(NTrks == 0) LOG(fatal)<<"UpdateTrackList NTrks=0 for event "<< fFindTracks->GetEventNumber()<<", pTrk "
			     <<pTrk<<", iHit "<<iHit;
    if(NTrks > 0) {
      //PrintStatus((char*)"UpdateTrackList::cleanup1");
      Int_t iterClean=1;
      while(iterClean>0){
	LOG(debug2) << " <D1> UpdateTrackList for Trk "<<pTrk
		    <<Form(", %d.Hit(%d) at ind %d with %d(%d) registered tracks",
			  iHit,pTrk->GetNofHits(),iHitInd,(int)fvTrkVec[iHitInd].size(),NTrks);
	//if(fvTrkVec[iHitInd].size()==1) break;
	for(std::vector<CbmTofTracklet*>::iterator iT=fvTrkVec[iHitInd].begin(); iT!=fvTrkVec[iHitInd].end(); iT++){
	  iterClean=0;	 
	  if(!Active(*iT)) break; // check whether tracklet is still active
	  LOG(debug2) << " <D2>  process Trk "<<*iT<<" with "<<(*iT)->GetNofHits()<<" hits";

	  for(Int_t iH=0; iH<(*iT)->GetNofHits(); iH++){
	    if(!Active(*iT)) break; // check whether tracklet is still active
	    Int_t iHi = (*iT)->GetTofHitIndex(iH);
	    LOG(debug2) << " <D3>  process Hit "<<iH<<" at index "<<iHi;
	    Int_t iAddri = ( (*iT)->GetTofHitPointer(iH)->GetAddress() & DetMask );	     
	    LOG(debug2) <<"   --- iHitInd "<<iHitInd<<"("<<fvTrkVec.size()<<"), size "<<fvTrkVec[iHitInd].size()
			<<" - iH "
			<<iH<<"("<<(*iT)->GetNofHits()<<"), iHi "<<iHi<<" Hi vec size "<<fvTrkVec[iHi].size()
			<<Form(" poi %p, iTpoi %p, SmAddr 0x%08x, 0x%08x, 0x%08x ", pTrk, *iT, (*iT)->GetTofHitPointer(iH)->GetAddress(),iAddri,fFindTracks->GetBeamCounter());

	    if(iAddri==fFindTracks->GetBeamCounter()) {
	      LOG(debug2) <<" Hit in beam counter, continue ...";
	      continue; 
	    }
	    if(fvTrkVec[iHi].size()==0) {
	      LOG(fatal)<<"CbmTofTrackFinderNN::UpdateTrackList no track "
			<<" for hit "<<iH<<", Hind "<<iHi
			<<", size "<<fvTrkVec[iHi].size();
	      break;
	    }
	    else{       // loop over tracks  referenced by hit iHi 
	      for(std::vector<CbmTofTracklet*>::iterator it=fvTrkVec[iHi].begin();  it!=fvTrkVec[iHi].end(); it++){
		LOG(debug2) << "    UpdateTrackList for pTrk "<<pTrk<<" <-> "<<*iT<<" <-> "<<*it<<", clean "<<iterClean
			    << ", hit "<<iHi<<", size "<<fvTrkVec[iHi].size();                 
		if(*it != pTrk) { 
		  size_t iTr=0;;
		  for(iTr=0; iTr<fTracks.size(); iTr++){
		    if (fTracks[iTr] == *it) {
		      LOG(debug2)<<Form("    found track entry %p(%d) at %lu of iHi %d, pTrk %p",
					*it,(int)fvTrkVec[iHi].size(),iTr,iHi, pTrk);  
		      break;	
		    }
		  }

		  if(iTr == fTracks.size()){
		    LOG(fatal) << "CbmTofTrackFinderNN::UpdateTrackList: Invalid iTr for pTrk "
			       <<pTrk<<", iTr "<<iTr<<", size "<<fvTrkVec[iHi].size();
		    break;		     
		  }

		  LOG(debug2)<<Form("<D4> number of registered hits %3d at %p while keeping iHi = %d, pTrk at %p",
				    (*it)->GetNofHits(),(*it),iHi,pTrk);

		  CbmTofTracklet* pKill = *it;
		  // remove track link registered for each associated hit to the track that is going to be removed
		  for(Int_t iht=0; iht<pKill->GetNofHits();iht++) {
		    Int_t iHI=pKill->GetHitIndex(iht); 
		    LOG(debug2)<<Form("<D5> remove track link %p for hit iHi = %d, loop index %d: iHI = %3d ",pKill,iHi,iht,iHI);  

		    for(std::vector<CbmTofTracklet*>::iterator itt=fvTrkVec[iHI].begin(); itt!=fvTrkVec[iHI].end(); itt++){
		      if( (*itt) == pTrk  ) continue;
		      if( (*itt) == pKill ) {
			LOG(debug2)<<Form("<D6> remove track link %p for hit iHi = %d, iHI = %3d, #Trks %3d",
					  pKill,iHi,iHI,(int)fvTrkVec[iHI].size()); 
			if(fvTrkVec[iHI].size() == 1) {
			  LOG(debug2)<<"<D6a> clear vector fvTrkVec for "<< iHI; 
			  fvTrkVec[iHI].clear(); 
			  //  it =fvTrkVec[iHi].begin();
			  break;
			}else {
			  itt=fvTrkVec[iHI].erase(itt); // costly operation
			  LOG(debug2)<<"<D6b> reduce fvTrkVec size of " << iHI << " to " << fvTrkVec[iHI].size();
			  break;
			}
		      }
		    }
		    LOG(debug2)<<Form("<D7> removed track link %p for hit iHi = %d, loop %d: iHI = %3d ",pKill,iHi,iht,iHI);  

		    // PrintStatus((char*)"UpdateTrackList::Remove1");
		  }	// loop on associated hits end
		  pKill->Delete();     //delete tracklet *it;
                  LOG(debug2)<<"<D8> remove tracklet at pos " << iTr; 
		  fTracks.erase(fTracks.begin()+iTr);
		  fiNtrks--;

		  LOG(debug2) << "Erase1 for pTrk "<<pTrk<<", at "<<iTr
			      <<", hit "<<iHi<<", size "<<fvTrkVec[iHi].size() ;

		  PrintStatus((char*)"UpdateTrackList::Erase1");

		  /*
		   if(fvTrkVec[iHi].size() == 1) {
		     fvTrkVec[iHi].clear();
		     LOG(debug2) << "  clear1 for pTrk "<<pTrk<<", hit "<<iHi<<", size "<<fvTrkVec[iHi].size()
				 ;
		     goto loopclean; 
		   }else{
		     it=fvTrkVec[iHi].erase(it);
		     //NTrks--;
		     LOG(debug2) << "    erase3 for "<<iTrk<<" at "<<pTrk<<", hit "<<iHi
				 <<", size "<<fvTrkVec[iHi].size()<<", "<<NTrks
			         ;		   
		   }

		  */
		  //if(iHi == iHitInd) NTrks--;
		  //PrintStatus((char*)"UpdateTrackList::cleanup2");
		  iterClean=2;
		  break;
		}else {   // *it==pTrk
		  if(fvTrkVec[iHi].size()<2) break;
		  // if(pTrk == *iT) goto loopclean;  // 
		}
	      }   // end of loop over tracks referenced by hit iHi  
	    }
	  }
	}
      }
    }
  }
}

void CbmTofTrackFinderNN::PrintStatus(char* cComment)
{
  LOG(debug)<<Form("<PS %s> for fiNtrks = %d tracks out of %d fTracks.size() ",cComment,fiNtrks,(int)fTracks.size())
            ;

  for (size_t it=0; it<fTracks.size(); it++){
    CbmTofTracklet* pTrk = (CbmTofTracklet *)fTracks[it];
    if(NULL == pTrk) continue;
    if(pTrk->GetNofHits()<2) {
      LOG(fatal) << "Invalid track found";
    }

    TString sTrk="";
    sTrk += Form("  Track %lu at %p, Hits: ", it, pTrk);
    for(Int_t ih=0; ih<pTrk->GetNofHits();ih++){
	sTrk += Form(" %3d ",pTrk->GetHitIndex(ih));
    }
    sTrk += Form(", ChiSq %7.1f",pTrk->GetChiSq());
    sTrk += Form(", Tt %6.4f",pTrk->GetTt());
    LOG(debug) << sTrk;
  }

  for (size_t ih=0; ih<fvTrkVec.size(); ih++){
    CbmTofHit* pHit = (CbmTofHit*) fHits->At( ih );
    Int_t iAddr = (pHit->GetAddress() & DetMask );
    Int_t iSt   = fFindTracks->GetStationOfAddr(iAddr);
    TString sTrk="";
    sTrk +=Form("    Hit %lu, A 0x%08x, St %d, T %6.2f, Tracks(%d): ",
		ih, pHit->GetAddress(), iSt, pHit->GetTime(), (int)fvTrkVec[ih].size());    
    if(iSt < fFindTracks->GetNStations()){
     for (size_t it=0; it<fvTrkVec[ih].size(); it++){
      CbmTofTracklet* pTrk = fvTrkVec[ih][it];
       sTrk += Form(" %p ",pTrk);
     }
     LOG(debug) << sTrk;
    }
  }
}

Bool_t CbmTofTrackFinderNN::Active(CbmTofTracklet*  pCheck)
{
  for (size_t it=0; it<fTracks.size(); it++){
    CbmTofTracklet* pTrk = (CbmTofTracklet *)fTracks[it];
    if(NULL == pTrk) continue;
    if(pCheck == pTrk) return kTRUE;
  }
  return kFALSE;
}

void CbmTofTrackFinderNN::Line3Dfit(CbmTofTracklet*  pTrk)
{
   TGraph2DErrors * gr = new TGraph2DErrors();
  
   // Fill the 2D graph
   // generate graph with the 3d points
   for (Int_t N=0; N<pTrk->GetNofHits(); N++) {
      double x,y,z = 0;
      x = (pTrk->GetTofHitPointer(N))->GetX();  
      y = (pTrk->GetTofHitPointer(N))->GetY();  
      z = (pTrk->GetTofHitPointer(N))->GetZ();  
      gr->SetPoint(N,x,y,z);
      double dx,dy,dz = 0.;
      dx = (pTrk->GetTofHitPointer(N))->GetDx();  
      dy = (pTrk->GetTofHitPointer(N))->GetDy();  
      dz = (pTrk->GetTofHitPointer(N))->GetDz(); //FIXME
      gr->SetPointError(N,dx,dy,dz);
      LOG(debug) << "Line3Dfit add N = "<<N<<",\t"<<pTrk->GetTofHitIndex(N)<<",\t"
		 <<x<<",\t"<<y<<",\t"<<z<<",\t"<<dx<<",\t"<<dy<<",\t"<<dz;
   }  
   // fit the graph now 
   Double_t pStart[4]= {0.,0.,0.,0.};
   pStart[0]=pTrk->GetFitX(0.);
   pStart[1]=(pTrk->GetTrackParameter())->GetTx();
   pStart[2]=pTrk->GetFitY(0.);
   pStart[3]=(pTrk->GetTrackParameter())->GetTy();
   LOG(debug) << "Line3Dfit init: X0 "
	      <<pStart[0]<<", TX "<<pStart[1]<<", Y0 "<<pStart[2]<<", TY "<<pStart[3];
   
   fMinuit.DoFit(gr,pStart);
   //gr->Draw("err p0");
   gr->Delete();
   Double_t* dRes;
   dRes=fMinuit.GetParFit();
   LOG(debug) << "Line3Dfit result: "
	      <<gMinuit->fCstatu<<" : X0 "
	      <<dRes[0]<<", TX "<<dRes[1]<<", Y0 "<<dRes[2]<<", TY "<<dRes[3]
	      << ", Chi2DoF: "<< fMinuit.GetChi2DoF();
	      
   (pTrk->GetTrackParameter())->SetX( dRes[0] );
   (pTrk->GetTrackParameter())->SetY( dRes[2] );
   (pTrk->GetTrackParameter())->SetZ( 0. );
   (pTrk->GetTrackParameter())->SetTx( dRes[1] );
   (pTrk->GetTrackParameter())->SetTy( dRes[3] );
   (pTrk->GetTrackParameter())->SetQp( 1. ); // FIXME
   //  pTrk->SetChiSq(fMinuit.GetChi2DoF());
   pTrk->SetChiSq(fMinuit.GetChi2DoF()/pTrk->GetNofHits());  // empirical to equilibrate bias on hit multiplicity!!!
}

ClassImp(CbmTofTrackFinderNN)
