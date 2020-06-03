/*
 * NicaCbmTrack.cxx
 *
 *  Created on: 11 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "NicaCbmTrack.h"
#include "NicaCbmEvent.h"
#include "NicaCbmEventInterface.h"
#include "NicaTrackClones.h"

NicaCbmTrack::NicaCbmTrack():fStsTrack(NULL),
fTofTrack(NULL),fTrdTrack(NULL),fRichRing(NULL),fMuchTrack(NULL),
fStsTrackIndex(-1),fTofTrackIndex(-1),fTrdTrackIndex(-1),fRichRingIndex(-1),
fMuchTrackIndex(-1){
}

NicaCbmTrack::~NicaCbmTrack() {
}

void NicaCbmTrack::Update(CbmGlobalTrack* track, NicaCbmGlobalEventInterface *interface){
	NicaCbmEventInterface *evint  = (NicaCbmEventInterface*)interface;
	NicaCbmGlobalTrack::Update(track,interface);
	fStsTrackIndex = track->GetStsTrackIndex();
	fTofTrackIndex = track->GetTofHitIndex();
	fTrdTrackIndex = track->GetTrdTrackIndex();
	NicaCbmSetup mode = evint->GetMode();
	if(mode==kSis100Electron)
		fRichRingIndex = track->GetRichRingIndex();
	else if(mode==kSis100Muon)
		fMuchTrackIndex = track->GetMuchTrackIndex();
	if(fStsTrackIndex>=0)
		SetStsTrack((CbmStsTrack*)evint->GetStsTracks()->UncheckedAt(fStsTrackIndex));
	else
		SetStsTrack(NULL);
	if(fTofTrackIndex>=0){
		CbmTofHit *hit = (CbmTofHit*)evint->GetTofHits()->UncheckedAt(fTofTrackIndex);
		SetTofTrack(hit);
		Double_t t = hit->GetTime();
		Double_t beta =  GetTrackLenght()/t/(29.9792458);
		Double_t p =GetMomentum()->P();
		Double_t m2  = p*p*(1./beta/beta-1.);
		if(m2>0){
			SetMass(TMath::Sqrt(m2));
		}else{
			SetMass(TMath::Sqrt(-m2));
		}
	}else{
		SetTofTrack(NULL);
		SetMass(-2);
	}
	if(fTrdTrackIndex>=0)
		SetTrdTrack((CbmTrdTrack*)evint->GetTrdTracks()->UncheckedAt(fTrdTrackIndex));
	else
		SetTrdTrack(NULL);
	if(fRichRingIndex>=0)
		SetRichRing((CbmRichRing*)evint->GetRichRings()->UncheckedAt(fRichRingIndex));
	else
		SetRichRing(NULL);
	if(fMuchTrackIndex>=0)
		SetMuchTrack((CbmMuchTrack*)evint->GetMuchTracks()->UncheckedAt(fMuchTrackIndex));
	else
		SetMuchTrack(NULL);

}

void NicaCbmTrack::CopyData(NicaTrack *track) {
	NicaCbmGlobalTrack::CopyData(track);
	NicaCbmTrack *other = (NicaCbmTrack*)track;
	fStsTrackIndex = other->fStsTrackIndex;
	fTofTrackIndex = other->fTofTrackIndex;
	fTrdTrackIndex = other->fTrdTrackIndex;
	fRichRingIndex = other->fRichRingIndex;
	fMuchTrackIndex = other->fMuchTrackIndex;
	NicaCbmEvent *ev = (NicaCbmEvent*)GetEvent();
	if(fStsTrackIndex>=0)
		fStsTrack = (CbmStsTrack*)ev->GetStsTracks()->UncheckedAt(fStsTrackIndex);
	else
		fStsTrack = NULL;
	if(fTofTrackIndex>=0)
		fTofTrack = (CbmTofHit*)ev->GetTofHits()->UncheckedAt(fTofTrackIndex);
	else
		fTofTrack = NULL;
	if(fTrdTrackIndex>=0)
		fTrdTrack =(CbmTrdTrack*) ev->GetTrdTracks()->UncheckedAt(fTrdTrackIndex);
	else
		fTrdTrack = NULL;
	if(fRichRingIndex>=0)
		fRichRing = (CbmRichRing*)ev->GetRichRings()->UncheckedAt(fRichRingIndex);
	else
		fRichRing = NULL;
	if(fMuchTrackIndex>=0)
		fMuchTrack =(CbmMuchTrack*) ev->GetMuchTracks()->UncheckedAt(fMuchTrackIndex);
	else
		fMuchTrack = NULL;
}
