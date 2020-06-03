/*
 * CbmTofTrackCut.cxx
 *
 *  Created on: 17 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CbmHasTofTrackCut.h"

#include "NicaCbmTrack.h"
CbmHasTofTrackCut::CbmHasTofTrackCut() :NicaTrackCut(1){
}

Bool_t CbmHasTofTrackCut::Init(Int_t format_id) {
	fInit = kTRUE;
	if(FormatInhertis("NicaCbmEvent", format_id)){
		return kTRUE;
	}
	return kFALSE;
}

Bool_t CbmHasTofTrackCut::Pass(NicaTrack *track) {
	NicaCbmTrack *tr = (NicaCbmTrack*)track;
	CbmTofHit *tof = tr->GetTofTrack();
	if(tof==NULL){
		SetValue(0);
		return ForcedUpdate(kFALSE);
	}else{
		SetValue(1);
		return ForcedUpdate(kTRUE);
	}
}

CbmHasTofTrackCut::~CbmHasTofTrackCut() {
	// TODO Auto-generated destructor stub
}

