/*
 * NicaBmnGlobalMCEvent.cxx
 *
 *  Created on: 20 wrz 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "NicaCbmGlobalMCEvent.h"

#include <stddef.h>
#include <TClonesArray.h>
#include <TObjArray.h>

#include "CbmLink.h"
#include "NicaComplexTrack.h"
#include "NicaEvent.h"
#include "NicaCbmGlobalTrack.h"
#include "NicaCbmMatchedEvent.h"
#include "NicaCbmMatchedEventInterface.h"
#include "NicaCbmMCEvent.h"
#include "NicaCbmMCTrack.h"

void NicaCbmGlobalMCEvent::Update() {
	fImgEvent->Update();
	fRealEvent->Update();
	NicaEvent::ShallowCopyEvent(fRealEvent);
	fTracks->Clear();
	fTotalTracksNo = fRealEvent->GetTotalTrackNo();
	fTracks->ExpandCreateFast(fTotalTracksNo);
	NicaCbmMatchedEventInterface *interface = (NicaCbmMatchedEventInterface*)fRealEvent->GetSource();
	for(int i=0;i<fTotalTracksNo;i++){
		NicaComplexTrack *track = (NicaComplexTrack*)fTracks->UncheckedAt(i);
		track->SetRealTrack(fRealEvent->GetTrack(i));
		track->NicaTrack::CopyData(fRealEvent->GetTrack(i));
		//now lets look for MC track
		interface->FillTrackInterface(fRealTrackInterface, i);
		CbmTrackMatchNew *link = NULL;
		Int_t id_match;
		switch(fMatching){
			case ECbmModuleId::kSts:
				link = fRealTrackInterface->GetLinkSts();
				break;
			case ECbmModuleId::kTof:
				link = fRealTrackInterface->GetLinkTof();
				break;
			case ECbmModuleId::kTrd:
				link = fRealTrackInterface->GetLinkTrd();
				break;
			case ECbmModuleId::kRich:
				link = fRealTrackInterface->GetLinkRich();
				break;
			case ECbmModuleId::kMuch:
				link = fRealTrackInterface->GetLinkMuch();
				break;
			default:
				break;
		}
		id_match = link->GetMatchedLink().GetIndex();
		if(id_match<0){
			track->SetImgTrack(NULL);
		}else{
			NicaCbmMCTrack *mc = (NicaCbmMCTrack*)fImgEvent->GetTrack(id_match);
			track->SetImgTrack(mc);
			track->SetMatchID(id_match);
		}
	}
}

Bool_t NicaCbmGlobalMCEvent::ExistInTree() const {
	//NicaCbmMatchedEventInterface *interface = (NicaCbmMatchedEventInterface*)fRealEvent->GetSource();
	if(fImgEvent&&fRealEvent){
		if(fImgEvent->ExistInTree()&&fRealEvent->ExistInTree()){
			return kTRUE;
		}
	}
	return kFALSE;
}

NicaCbmGlobalMCEvent::NicaCbmGlobalMCEvent(NicaCbmSetup mode,
		ECbmModuleId match) :
		NicaComplexEvent(new NicaCbmMatchedEvent(mode),new NicaCbmMCEvent()),fMatching(match),fSetup(mode){
	fRealTrackInterface = new NicaCbmMatchedTrackInterface();
}

NicaEvent* NicaCbmGlobalMCEvent::GetNewEvent() const {
	return new NicaCbmGlobalMCEvent(
			((NicaCbmMatchedEvent*)fRealEvent)->GetMode(),
			fMatching);
}

NicaCbmGlobalMCEvent::~NicaCbmGlobalMCEvent() {
	delete fRealTrackInterface;
}

