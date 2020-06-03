/*
 * NicaUnigenSource.cxx
 *
 *  Created on: 2 sie 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "NicaUnigenEventInterface.h"
#include "FairRootManager.h"
#include "NicaUnigenTrackInterface.h"

NicaUnigenEventInterface::NicaUnigenEventInterface() :fEvent(NULL){
	fEvent = new UEvent();
}

void NicaUnigenEventInterface::Compress(Int_t* map, Int_t map_size) {
	Int_t track_pos = 0;
	for(int i=0;i<map_size;i++){
		Int_t good_track = map[i];
		for(int j=track_pos;j<good_track;j++){
			fEvent->RemoveAt(j);
		}
		track_pos = good_track+1;
	}
}

void NicaUnigenEventInterface::CopyData(NicaEventInterface* s) {
#ifdef UNIGEN_OLD
	CopyUnigen(((NicaUnigenEventInterface*)s)->fEvent, fEvent);
#else
	*fEvent = *((NicaUnigenEventInterface*)s)->fEvent;
#endif
}

void NicaUnigenEventInterface::CopyAndCompress(NicaEventInterface* s, Int_t* map,
		Int_t map_size) {
	NicaUnigenEventInterface *ev = (NicaUnigenEventInterface*)s;
	fEvent->SetB(ev->fEvent->GetB());
	fEvent->SetPhi(ev->fEvent->GetPhi());
	fEvent->SetNes(ev->fEvent->GetNes());
	fEvent->SetStepNr(ev->fEvent->GetStepNr());
	fEvent->SetStepT(ev->fEvent->GetStepT());
#ifdef UNIGEN_OLD
	fEvent->GetParticleList()->Clear();
#else
	TString comment;
	ev->fEvent->GetComment(comment);
	fEvent->SetComment(comment);
	fEvent->Clear();
#endif
	for(int i=0;i<map_size;i++){
		fEvent->AddParticle(*ev->fEvent->GetParticle(map[i]));
	}
}

void NicaUnigenEventInterface::ConnectToTree() {
	FairRootManager *manager = FairRootManager::Instance();
	if(CanDeleteEvent()){
		if(fEvent) delete fEvent;
	}
	fEvent = (UEvent*)manager->GetObject("UEvent.");
}

void NicaUnigenEventInterface::Boost(Double_t vx, Double_t vy, Double_t vz) {
	for(int i=0;i<fEvent->GetNpa();i++){
		UParticle *p = fEvent->GetParticle(i);
		TLorentzVector mom = p->GetMomentum();
		TLorentzVector pos = p->GetPosition();
		mom.Boost(vx,vy,vz);
		pos.Boost(vx,vy,vz);
		p->SetMomentum(mom);
		p->SetPosition(pos);
	}
}

NicaUnigenEventInterface::~NicaUnigenEventInterface() {
	if(CanDeleteEvent()) {
		if(fEvent) delete fEvent;
	}
}

NicaTrackInterface *NicaUnigenEventInterface::GetTrackInterface() const {
	return new NicaUnigenTrackInterface();
}

void NicaUnigenEventInterface::Register(Bool_t write) {
	if(fEvent==NULL) fEvent = new UEvent();
	FairRootManager *manager = FairRootManager::Instance();
	manager->Register("Event","",(TNamed*)fEvent,write);
}

void NicaUnigenEventInterface::FillTrackInterface(NicaTrackInterface* track,
		Int_t index) {
	track->SetRawTrack(fEvent->GetParticle(index));
}
#ifdef UNIGEN_OLD
void NicaUnigenEventInterface::CopyUnigen(UEvent *from, UEvent *to) {
	to->GetParticleList()->Clear();
	to->SetB(from->GetB());
	to->SetPhi(from->GetPhi());
	to->SetNes(from->GetNes());
	to->SetStepNr(from->GetStepNr());
	to->SetStepT(from->GetStepT());
	for(int i=0;i<from->GetNpa();i++){
		to->AddParticle(*from->GetParticle(i));
	}
}
#endif
