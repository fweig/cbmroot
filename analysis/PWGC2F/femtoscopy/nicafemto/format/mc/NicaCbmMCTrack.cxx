/*
 * NicaCbmMCTrack.cxx
 *
 *  Created on: 20 wrz 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "NicaCbmMCTrack.h"

#include <FairLogger.h>

NicaCbmMCTrack::NicaCbmMCTrack() :fNPoints(0){
}

void NicaCbmMCTrack::Update(CbmMCTrack* mc,Double_t charge) {
	GetMomentum()->SetPxPyPzE(mc->GetPx(), mc->GetPy(), mc->GetPz(), mc->GetEnergy());
	SetMotherIndex(mc->GetMotherId());
	if(GetMotherIndex()>-1){
		SetPrimary(kFALSE);
	}else{
		SetPrimary(kTRUE);
	}
	SetStatus(0);
	SetCharge(charge);
	SetPdg(mc->GetPdgCode());
	GetStartPosition()->SetXYZT(mc->GetStartX(),mc->GetStartY(),mc->GetStartZ(),mc->GetStartT());
	SetNPoints(ECbmModuleId::kRef, mc->GetNPoints(ECbmModuleId::kRef));
	SetNPoints(ECbmModuleId::kMvd, mc->GetNPoints(ECbmModuleId::kMvd));
	SetNPoints(ECbmModuleId::kSts, mc->GetNPoints(ECbmModuleId::kSts));
	SetNPoints(ECbmModuleId::kRich, mc->GetNPoints(ECbmModuleId::kRich));
	SetNPoints(ECbmModuleId::kMuch, mc->GetNPoints(ECbmModuleId::kMuch));
	SetNPoints(ECbmModuleId::kTrd, mc->GetNPoints(ECbmModuleId::kTrd));
	SetNPoints(ECbmModuleId::kTof, mc->GetNPoints(ECbmModuleId::kTof));
	SetNPoints(ECbmModuleId::kEcal, mc->GetNPoints(ECbmModuleId::kEcal));
	SetNPoints(ECbmModuleId::kPsd, mc->GetNPoints(ECbmModuleId::kPsd));
}

NicaCbmMCTrack::~NicaCbmMCTrack() {
}

void NicaCbmMCTrack::SetNPoints(ECbmModuleId iDet, Int_t nPoints) {
	switch(iDet){
		case ECbmModuleId::kRef:{
		    if      ( nPoints < 0 ) nPoints = 0;
		    else if ( nPoints > 1 ) nPoints = 1;
		    fNPoints = ( fNPoints & ( ~ 1 ) )  |  nPoints;
		}break;
		case ECbmModuleId::kMvd:{
		    if      ( nPoints < 0 ) nPoints = 0;
		    else if ( nPoints > 7 ) nPoints = 7;
		    fNPoints = ( fNPoints & ( ~ (  7 <<  1 ) ) )  |  ( nPoints <<  1 );
		}break;
		case ECbmModuleId::kSts:{
		    if      ( nPoints <  0 ) nPoints =  0;
		    else if ( nPoints > 31 ) nPoints = 31;
		    fNPoints = ( fNPoints & ( ~ ( 31 <<  4 ) ) )  |  ( nPoints <<  4 );
		}break;
		case ECbmModuleId::kRich:{
		    if      ( nPoints < 0 ) nPoints = 0;
		    else if ( nPoints > 1 ) nPoints = 1;
		    fNPoints = ( fNPoints & ( ~ (  1 <<  9 ) ) )  |  ( nPoints <<  9 );
		}break;
		case ECbmModuleId::kMuch:{
			if ( nPoints <  0 ) nPoints =  0;
			else if ( nPoints > 31 ) nPoints = 31;
			fNPoints = ( fNPoints & ( ~ ( 31 << 10 ) ) )  |  ( nPoints << 10 );
		}break;
		case ECbmModuleId::kTrd:{
			 if( nPoints <  0 ) nPoints =  0;
			 else if ( nPoints > 31 ) nPoints = 31;
			 fNPoints = ( fNPoints & ( ~ ( 31 << 15 ) ) )  |  ( nPoints << 15 );
		}break;
		case ECbmModuleId::kTof:{
			 if ( nPoints <  0 ) nPoints =  0;
			 else if ( nPoints > 15 ) nPoints = 15;
			 fNPoints = ( fNPoints & ( ~ ( 15 << 20 ) ) )  |  ( nPoints << 20 );
		}break;
		case ECbmModuleId::kEcal:{
			if( nPoints < 0 ) nPoints = 0;
			else if ( nPoints > 1 ) nPoints = 1;
			fNPoints = ( fNPoints & ( ~ (  1 << 24 ) ) )  |  ( nPoints << 24 );
		}break;
		case ECbmModuleId::kPsd:{
			 if ( nPoints < 0 ) nPoints = 0;
			 else if ( nPoints > 1 ) nPoints = 1;
		    fNPoints = ( fNPoints & ( ~ (  1 << 25 ) ) )  |  ( nPoints << 25 );
		}break;
                default:{
                  LOG(warn) << "Call NicaCbmMCTrack::SetNPoints with unsupported system";
                }
	}
}

Int_t NicaCbmMCTrack::GetNPoints(ECbmModuleId detId) const {
	switch(detId){
	case ECbmModuleId::kRef:
		return (  fNPoints &   1);
		break;
	case ECbmModuleId::kMvd:{
		return ( (fNPoints & ( 7 <<  1) ) >>  1);
	}break;
	case ECbmModuleId::kSts:{
		return ( (fNPoints & (31 <<  4) ) >>  4);
	}break;
	case ECbmModuleId::kRich:{
		return ( (fNPoints & ( 1 <<  9) ) >>  9);
	}break;
	case ECbmModuleId::kMuch:{
		return ( (fNPoints & (31 << 10) ) >> 10);
	}break;
	case ECbmModuleId::kTrd:{
		return ( (fNPoints & (31 << 15) ) >> 15);
	}break;
	case ECbmModuleId::kTof:{
		return ( (fNPoints & (15 << 20) ) >> 20);
	}break;
	case ECbmModuleId::kEcal:{
		return ( (fNPoints & ( 1 << 24) ) >> 24);
	}break;
	case ECbmModuleId::kPsd:{
		return ( (fNPoints & ( 1 << 25) ) >> 25);
	}break;
	default:
		return 0;
	break;
	}
}

void NicaCbmMCTrack::CopyData(NicaTrack *other) {
	NicaMCTrack::CopyData(other);
	this->fNPoints = ((NicaCbmMCTrack*)other)->fNPoints;
}
