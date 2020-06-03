/** @file CbmTofCalibrator.cxx
 ** @author nh
 ** @date 28.02.2020
 **
 **/

// CBMroot classes and includes
#include "CbmTofCalibrator.h"
#include "CbmTofTracklet.h"
#include "CbmTofHit.h"
#include "CbmTofGeoHandler.h" // in tof/TofTools
#include "CbmTofDetectorId_v12b.h" // in cbmdata/tof
#include "CbmTofDetectorId_v14a.h" // in cbmdata/tof
#include "CbmTofCell.h"       // in tof/TofData
#include "CbmTofDigiPar.h"    // in tof/TofParam
#include "CbmTofDigiBdfPar.h" // in tof/TofParam
#include "CbmTofAddress.h"    // in cbmdata/tof
#include "CbmTofClusterizersDef.h"
#include "CbmMatch.h"

// FAIR classes and includes
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

// ROOT Classes and includes
#include <TDirectory.h>
#include <TROOT.h>
#include <TGeoManager.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <TClonesArray.h>
#include <TFile.h>

CbmTofCalibrator::CbmTofCalibrator():
  fDigiMan(NULL),
  fTofClusterizer(NULL),
  fTofFindTracks(NULL),
  fTrackletTools(NULL),
  fDigiPar(NULL),
  fDigiBdfPar(NULL),
  fTofDigiMatchColl(NULL),
  fhCalPos(),
  fhCalTOff(),
  fhCalTot(),
  fhCalWalk(),
  fhCorPos(),
  fhCorTOff(),
  fhCorTot(),
  fhCorTotOff(),
  fhCorSvel(),
  fhCorWalk(),
  fDetIdIndexMap()
{
}

CbmTofCalibrator::~CbmTofCalibrator() {
}

InitStatus CbmTofCalibrator::Init() {
  
   FairRootManager *fManager = FairRootManager::Instance();
  // check for availability of digis
  fDigiMan = CbmDigiManager::Instance();
  if( NULL == fDigiMan ) { LOG(warning) << "No CbmDigiManager"; return kFATAL;}
  fDigiMan->Init();
  
  if ( ! fDigiMan->IsPresent(ECbmModuleId::kTof) ) {
    LOG(warn)  <<  "CbmTofCalibrator: No digi input!";
  }

  // check for access to current calibration file
  fTofClusterizer = CbmTofEventClusterizer::Instance();
  if( NULL == fTofClusterizer ) {
    LOG(warn) << "CbmTofCalibrator: no access to active calibration";
  } else {
    TString CalParFileName=fTofClusterizer->GetCalParFileName();
    LOG(info) << "Current calibration file: " << CalParFileName;
  }
  fTofFindTracks = CbmTofFindTracks::Instance();
  if(NULL == fTofFindTracks) {
    LOG(warn)  << "CbmTofCalibrator: no access to tof tracker ";
  }

  // Get Access to MatchCollection
  fTofDigiMatchColl = (TClonesArray* )fManager->GetObject("TofDigiMatch");
  if(NULL == fTofDigiMatchColl) 
    fTofDigiMatchColl = (TClonesArray* )fManager->GetObject("TofCalDigiMatch");
  
  if( NULL == fTofDigiMatchColl ) {
    LOG(error) << "CbmTofCalibrator: no access to DigiMatch ";
    return kFATAL;
  }  
   
  // Get Parameters
  if ( !InitParameters() ) {
    LOG(error) << "CbmTofCalibrator: No parameters!";
  }
  // generate deviation histograms
  if ( !CreateCalHist() ) {
    LOG(error) << "CbmTofCalibrator: No histograms!";
    return kFATAL;
  }

  fTrackletTools = new CbmTofTrackletTools(); // initialize tools

  LOG(info) << "TofCalibrator initialized successfully";
  return kSUCCESS;
}

Bool_t CbmTofCalibrator::InitParameters( ){
  LOG(info) << "InitParameters: get par pointers from RTDB";
  // Get Base Container
  FairRun* ana = FairRun::Instance();
  FairRuntimeDb* rtdb=ana->GetRuntimeDb();

   fDigiPar = (CbmTofDigiPar*) (rtdb->getContainer("CbmTofDigiPar"));
   if ( NULL == fDigiPar ) return kFALSE;
   
   fDigiBdfPar = (CbmTofDigiBdfPar*) (rtdb->getContainer("CbmTofDigiBdfPar"));
   if( NULL == fDigiBdfPar ) return kFALSE;
   
  return kTRUE;
}

Bool_t CbmTofCalibrator::CreateCalHist( ){
  // detector related distributions
  Int_t iNbDet=fDigiBdfPar->GetNbDet();
  LOG(info)<<"Define Calibrator histos for "<<iNbDet<<" detectors ";
  fhCalPos.resize( iNbDet  );
  fhCalTOff.resize( iNbDet  );
  fhCalTot.resize( iNbDet  );
  fhCalWalk.resize( iNbDet  );

  fDetIdIndexMap.clear();

  for(Int_t iDetIndx=0; iDetIndx<iNbDet; iDetIndx++){
    Int_t iUniqueId = fDigiBdfPar->GetDetUId( iDetIndx );
    fDetIdIndexMap[iUniqueId]=iDetIndx;

    Int_t iSmType   = CbmTofAddress::GetSmType( iUniqueId );
    Int_t iSmId     = CbmTofAddress::GetSmId( iUniqueId );
    Int_t iRpcId    = CbmTofAddress::GetRpcId( iUniqueId );
    Int_t iUCellId  = CbmTofAddress::GetUniqueAddress(iSmId,iRpcId,0,0,iSmType);
    CbmTofCell* fChannelInfo = fDigiPar->GetCell(iUCellId);
    if (NULL==fChannelInfo) {
      LOG(warning)<<"No DigiPar for Det "  <<Form("0x%08x", iUCellId);
      continue;
    }
    Double_t YDMAX=5; 
    fhCalPos[iDetIndx] =  new TH2F( 
				   Form("cal_SmT%01d_sm%03d_rpc%03d_Pos", iSmType, iSmId, iRpcId ),
				   Form("Clu position of Rpc #%03d in Sm %03d of type %d; Strip []; ypos [cm]", iRpcId, iSmId, iSmType ),
				   fDigiBdfPar->GetNbChan(iSmType,iRpcId),0,fDigiBdfPar->GetNbChan(iSmType,iRpcId),
				   99, -YDMAX,YDMAX);

    Double_t TSumMax=2.;
    //if(iSmType == 5) TSumMax *= 2.; // enlarge for diamond / beamcounter
    fhCalTOff[iDetIndx] =  new TH2F( 
				    Form("cal_SmT%01d_sm%03d_rpc%03d_TOff", iSmType, iSmId, iRpcId ),
				    Form("Clu TimeZero of Rpc #%03d in Sm %03d of type %d; Strip []; TOff [ns]", iRpcId, iSmId, iSmType ),
				    fDigiBdfPar->GetNbChan(iSmType,iRpcId),0,fDigiBdfPar->GetNbChan(iSmType,iRpcId),
				    99, -TSumMax,TSumMax );
	 
    Double_t TotMax=25.;
    fhCalTot[iDetIndx] =  new TH2F( 
				   Form("cal_SmT%01d_sm%03d_rpc%03d_Tot", iSmType, iSmId, iRpcId ),
				   Form("Clu Tot of Rpc #%03d in Sm %03d of type %d; StripSide []; TOT [a.u.]", iRpcId, iSmId, iSmType ),
				   2*fDigiBdfPar->GetNbChan(iSmType,iRpcId),0,2*fDigiBdfPar->GetNbChan(iSmType,iRpcId),
				   100, 0., TotMax);
    
    fhCalWalk[iDetIndx].resize( fDigiBdfPar->GetNbChan(iSmType,iRpcId) );
    for( Int_t iCh=0; iCh<fDigiBdfPar->GetNbChan(iSmType,iRpcId); iCh++){
      fhCalWalk[iDetIndx][iCh].resize( 2 );
      for (Int_t iSide=0; iSide<2; iSide++) {
	fhCalWalk[iDetIndx][iCh][iSide]= new TH2D( 
			  Form("cal_SmT%01d_sm%03d_rpc%03d_Ch%03d_S%01d_Walk", iSmType, iSmId, iRpcId, iCh, iSide ),
                          Form("Walk in SmT%01d_sm%03d_rpc%03d_Ch%03d_S%01d_Walk; Tot [a.u.];  #DeltaT [ns]", iSmType, iSmId, iRpcId, iCh, iSide ),
                          nbClWalkBinX,0.,TotMax,nbClWalkBinY,-TSumMax,TSumMax );
      }
    }
  }
  
  return kTRUE;
}

void CbmTofCalibrator::FillCalHist( CbmTofTracklet *pTrk){
  // fill deviation histograms on walk level
  if(pTrk->GetTt() < 0) return;                                   // take tracks with positive velocity only
  if( ! pTrk->ContainsAddr( 0x00005006 ) ) return; // request beam counter hit for calibration
  
  for (Int_t iHit=0; iHit<pTrk->GetNofHits(); iHit++) {
    CbmTofHit* pHit=pTrk->GetTofHitPointer(iHit);
    Int_t iDetId = (pHit->GetAddress() & DetMask);
    Int_t iSmType = CbmTofAddress::GetSmType( iDetId );
    Int_t iSm     = CbmTofAddress::GetSmId( iDetId );
    Int_t iRpc    = CbmTofAddress::GetRpcId( iDetId );
   
    std::map<UInt_t,UInt_t>::iterator it=fDetIdIndexMap.find(iDetId);
    if (it == fDetIdIndexMap.end()) continue; // continue for invalid detector index
    Int_t iDetIndx=it->second;                         //fDetIdIndexMap[iDetId];

    Int_t iChId = pHit->GetAddress();
    Int_t iCh = CbmTofAddress::GetChannelId(iChId);
    CbmTofCell* fChannelInfo = fDigiPar->GetCell( iChId );
    if(NULL == fChannelInfo){
      LOG(error) << "Invalid Channel Pointer for ChId "  << Form(" 0x%08x ",iChId)<<", Ch "<<iCh;
      continue;
    }
    gGeoManager->FindNode(fChannelInfo->GetX(),fChannelInfo->GetY(),fChannelInfo->GetZ());
    Double_t hitpos[3];
    hitpos[0]=pHit->GetX();    hitpos[1]=pHit->GetY();    hitpos[2]=pHit->GetZ();
    Double_t hlocal_p[3];
//    TGeoNode* cNode= gGeoManager->GetCurrentNode();
    gGeoManager->MasterToLocal(hitpos, hlocal_p);
    hitpos[0]=pTrk->GetFitX(pHit->GetZ());    hitpos[1]=pTrk->GetFitY(pHit->GetZ());
    Double_t hlocal_f[3];
    gGeoManager->MasterToLocal(hitpos, hlocal_f);
   
    fhCalPos[iDetIndx]->Fill((Double_t)iCh,hlocal_p[1]-hlocal_f[1]);                     // transformed into LRF
    fhCalTOff[iDetIndx]->Fill((Double_t)iCh,pHit->GetTime()-pTrk->GetFitT(pHit->GetZ() ) ); // residuals transformed into LRF
    //fhCalTOff[iDetIndx]->Fill((Double_t)iCh,fTrackletTools->GetTdif(pTrk, iDetId, pHit));   // prediction by other hits

    Int_t iMA=pTrk->GetTofHitIndex(iHit);
    if(iMA > fTofDigiMatchColl->GetEntries()){
      LOG(error)<<" Inconsistent DigiMatches for Hitind "
		<<iMA<<", TClonesArraySize: "<<fTofDigiMatchColl->GetEntries();
    }
    CbmMatch* digiMatch=(CbmMatch *)fTofDigiMatchColl->At(iMA);
    
    for (Int_t iLink=0; iLink<digiMatch->GetNofLinks(); iLink+=2) {  // loop over digis
      CbmLink L0 = digiMatch->GetLink(iLink);   
      Int_t iDigInd0=L0.GetIndex(); 
      Int_t iDigInd1=(digiMatch->GetLink(iLink+1)).GetIndex();
      
      const CbmTofDigi* tDigi0 = fDigiMan->Get<CbmTofDigi>(iDigInd0);
      Int_t iCh0    = tDigi0->GetChannel();
      Int_t iSide0 = tDigi0->GetSide();
      fhCalWalk[iDetIndx][iCh0][iSide0]->Fill(tDigi0->GetTot(),tDigi0->GetTime()-fTrackletTools->GetTexpected(pTrk, iDetId, pHit) 
    			                                                 -(1.-2.*tDigi0->GetSide())*hlocal_f[1]/fDigiBdfPar->GetSigVel(iSmType,iSm,iRpc)
					                                 +fTofFindTracks->GetTOff(iDetId)
					                                );

      /*      
      LOG(info)<<"TSRCS "<<iSmType<<iSm<<iRpc<<iCh<<iSide0<<Form(": digo0 %f, ex %f, prop %f, Off %f, res %f",
								tDigi0->GetTime(),
								fTrackletTools->GetTexpected(pTrk, iDetId, pHit) ,
								fTofFindTracks->GetTOff(iDetId),
								(1.-2.*tDigi0->GetSide())*hlocal_f[1]/fDigiBdfPar->GetSigVel(iSmType,iSm,iRpc),
								tDigi0->GetTime()-fTrackletTools->GetTexpected(pTrk, iDetId, pHit) 
								-(1.-2.*tDigi0->GetSide())*hlocal_f[1]/fDigiBdfPar->GetSigVel(iSmType,iSm,iRpc));
      */					
      
      const CbmTofDigi* tDigi1 = fDigiMan->Get<CbmTofDigi>(iDigInd1);
      Int_t iCh1    = tDigi1->GetChannel();
      Int_t iSide1 = tDigi1->GetSide();
      fhCalWalk[iDetIndx][iCh1][iSide1]->Fill(tDigi1->GetTot(),tDigi1->GetTime()-fTrackletTools->GetTexpected(pTrk, iDetId, pHit) 
    			                                                 -(1.-2.*tDigi1->GetSide())*hlocal_f[1]/fDigiBdfPar->GetSigVel(iSmType,iSm,iRpc)
					                                 +fTofFindTracks->GetTOff(iDetId)					      
					                                );
    }
   
  }
}

Bool_t CbmTofCalibrator::UpdateCalHist( Int_t iOpt){
  // get current calibration histos
  LOG(info) << "CbmTofCalibrator:: update histos from "
                 << "file " << CbmTofEventClusterizer::Instance()->GetCalParFileName();
  TFile* fCalParFile = new TFile(CbmTofEventClusterizer::Instance()->GetCalParFileName(),"");
  assert(fCalParFile);
  ReadHist(fCalParFile);
  
  // modify calibration histograms 
  for(Int_t iDetIndx=0; iDetIndx< fDigiBdfPar->GetNbDet(); iDetIndx++){
    Int_t iUniqueId  = fDigiBdfPar->GetDetUId( iDetIndx );
//    Int_t iSmAddr   = iUniqueId & DetMask; 
    Int_t iSmType  = CbmTofAddress::GetSmType( iUniqueId );
//    Int_t iSm           = CbmTofAddress::GetSmId( iUniqueId );
    Int_t iRpc          = CbmTofAddress::GetRpcId( iUniqueId );
    switch(iOpt) {
    case 0: // none
      break;
    case 1: // update channel mean
      {
	//LOG(info) << "Update Offsets for TSR "<<iSmType<<iSm<<iRpc;
//	TProfile *hpP = fhCalPos[iDetIndx]->ProfileX();
	TProfile *hpT = fhCalTOff[iDetIndx]->ProfileX();
	TH1* hCalT    = fhCalTOff[iDetIndx]->ProjectionX();
	//fhCorPos[iDetIndx]->Add((TH1 *)hpP,-1.);
	//fhCorTOff[iDetIndx]->Add((TH1*)hpT,-1.); 
	Double_t dAvOff=0.; //hpT->GetMean(2);
	for (Int_t iBin=0; iBin<fhCorTOff[iDetIndx]->GetNbinsX(); iBin++) {
	  Double_t dDt=hpT->GetBinContent(iBin+1);
	  Double_t dCorT=fhCorTOff[iDetIndx]->GetBinContent(iBin+1);
	  Double_t dCts=hCalT->GetBinContent(iBin+1);
	  if(iDetIndx == -1 ) {
	    LOG(info) << Form("Update %s: bin %02d, Cts: %d, Old %f, dev %f, av %f, new %f", fhCorTOff[iDetIndx]->GetName() ,
			      iBin, (Int_t)dCts, dCorT, dDt, dAvOff, dCorT- dDt + dAvOff); 
	  }
	  
	  fhCorTOff[iDetIndx]->SetBinContent(iBin+1,dCorT+dDt+dAvOff);
	}
      }
      break;
    case 2: // update individual channel walks
      const Double_t MinCounts = 10.;
      Int_t iNbCh = fDigiBdfPar->GetNbChan( iSmType, iRpc );
      for( Int_t iCh = 0; iCh < iNbCh; iCh++ )
      {
	for(Int_t iSide=0; iSide<2; iSide++) {
	  //LOG(info) << "Get walk histo pointer for TSRCS " << iSmType<<iSm<<iRpc<<iCh<<iSide;
	  TProfile *hpW = fhCalWalk[iDetIndx][iCh][iSide]->ProfileX();        // mean deviation
	  TH1* hCW        = fhCalWalk[iDetIndx][iCh][iSide]->ProjectionX(); // contributing counts

	  Double_t dCorT=0;
	  for (Int_t iBin=0; iBin<fhCorTOff[iDetIndx]->GetNbinsX(); iBin++) {
	    Double_t dCts=hCW->GetBinContent(iBin+1);
	    Double_t dWOff= fhCorWalk[iDetIndx][iCh][iSide]->GetBinContent(iBin+1); // current value
	    if (dCts > MinCounts) {
	      dCorT=hpW->GetBinContent(iBin+1);
	    }
	    fhCorWalk[iDetIndx][iCh][iSide]->SetBinContent(iBin+1,dWOff-dCorT); //set new value
	  }
	}
      }
      break;
    } //switch( iOpt) end  
  }
  
  TFile* fCalParFileNew = new TFile(Form("New_%s",fCalParFile->GetName()),"RECREATE");
  WriteHist(fCalParFileNew);
  
  return kTRUE;
}

void CbmTofCalibrator::ReadHist( TFile* fHist){
  LOG(info) << "Read Cor histos from file " << fHist->GetName();
  if(0 == fhCorPos.size()) {
    Int_t iNbDet=fDigiBdfPar->GetNbDet();
    //LOG(info) << "resize histo vector for " << iNbDet << " detectors ";
    fhCorPos.resize(iNbDet);
    fhCorTOff.resize(iNbDet);
    fhCorTot.resize(iNbDet);
    fhCorTotOff.resize(iNbDet);
    fhCorSvel.resize(iNbDet);
    fhCorWalk.resize(iNbDet);
  }
  
  for(Int_t iDetIndx=0; iDetIndx< fDigiBdfPar->GetNbDet(); iDetIndx++){
    Int_t iUniqueId  = fDigiBdfPar->GetDetUId( iDetIndx );
//    Int_t iSmAddr   = iUniqueId & DetMask; 
    Int_t iSmType  = CbmTofAddress::GetSmType( iUniqueId );
    Int_t iSm           = CbmTofAddress::GetSmId( iUniqueId );
    Int_t iRpc          = CbmTofAddress::GetRpcId( iUniqueId );
    //LOG(info) << "Get histo pointer for TSR " << iSmType<<iSm<<iRpc;
    
    fhCorPos[iDetIndx]   =(TH1 *) gDirectory->FindObjectAny( Form("cl_CorSmT%01d_sm%03d_rpc%03d_Pos_pfx",iSmType,iSm,iRpc));
    if (NULL == fhCorPos[iDetIndx]  ) {
      LOG(error) << "hCorPos not found";
      return;
    }
    fhCorTOff[iDetIndx] =(TH1 *) gDirectory->FindObjectAny( Form("cl_CorSmT%01d_sm%03d_rpc%03d_TOff_pfx",iSmType,iSm,iRpc));
    fhCorTot[iDetIndx]   =(TH1 *) gDirectory->FindObjectAny( Form("cl_CorSmT%01d_sm%03d_rpc%03d_Tot_Mean",iSmType,iSm,iRpc));
    fhCorTotOff[iDetIndx]  =(TH1 *) gDirectory->FindObjectAny( Form("cl_CorSmT%01d_sm%03d_rpc%03d_Tot_Off",iSmType,iSm,iRpc));

    Int_t iNbCh = fDigiBdfPar->GetNbChan( iSmType, iRpc );
    fhCorWalk[iDetIndx].resize(iNbCh);
    for( Int_t iCh = 0; iCh < iNbCh; iCh++ )
    {
      fhCorWalk[iDetIndx][iCh].resize(2);
      for(Int_t iSide=0; iSide<2; iSide++){
	//LOG(info) << "Get walk histo pointer for TSRCS " << iSmType<<iSm<<iRpc<<iCh<<iSide;
	fhCorWalk[iDetIndx][iCh][iSide]= (TH1 *)gDirectory->FindObjectAny( 
								Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S%d_Walk_px", iSmType, iSm, iRpc, iCh, iSide) );
      }
    }
  }
}

void CbmTofCalibrator::WriteHist( TFile* fHist){
  LOG(info) << "Write Cor histos to file " << fHist->GetName();
  TDirectory * oldir = gDirectory;
  fHist->cd();
  for(Int_t iDetIndx=0; iDetIndx< fDigiBdfPar->GetNbDet(); iDetIndx++){
    Int_t iUniqueId  = fDigiBdfPar->GetDetUId( iDetIndx );
//    Int_t iSmAddr   = iUniqueId & DetMask; 
    Int_t iSmType  = CbmTofAddress::GetSmType( iUniqueId );
//    Int_t iSm           = CbmTofAddress::GetSmId( iUniqueId );
    Int_t iRpc          = CbmTofAddress::GetRpcId( iUniqueId );

    fhCorPos[iDetIndx]->Write();
    fhCorTOff[iDetIndx]->Write();
    fhCorTot[iDetIndx]->Write();
    fhCorTotOff[iDetIndx]->Write();
      
    Int_t iNbCh = fDigiBdfPar->GetNbChan( iSmType, iRpc );
    fhCorWalk[iDetIndx].resize(iNbCh);
    for( Int_t iCh = 0; iCh < iNbCh; iCh++ )
    {
      fhCorWalk[iDetIndx][iNbCh].resize(2);
      for(Int_t iSide=0; iSide<2; iSide++){
	fhCorWalk[iDetIndx][iCh][iSide]->Write();
      }
    }
    
  }
  gDirectory = oldir;
}

ClassImp(CbmTofCalibrator)

