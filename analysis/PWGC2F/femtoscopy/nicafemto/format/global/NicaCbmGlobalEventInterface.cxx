/*
 * NicaCbmEventInterface.cxx
 *
 *  Created on: 3 gru 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "NicaCbmGlobalEventInterface.h"

#include <FairRootManager.h>
#include <RtypesCore.h>
#include <stddef.h>
#include <TObjArray.h>

#include "CbmStsTrack.h"
#include "CbmGlobalTrack.h"
#include "CbmTofTrack.h"

NicaCbmGlobalEventInterface::NicaCbmGlobalEventInterface() :fVertex(NULL),
fTracks(NULL){
	fVertex  = new CbmVertex();
	fTracks = new NicaTrackClones("CbmGlobalTrack","GlobalTrack","");
}

void NicaCbmGlobalEventInterface::ConnectToTree() {
	FairRootManager *manager = FairRootManager::Instance();
	if(CanDeleteEvent()){
		delete fVertex;
		fTracks->DeleteClones();
	}
	fVertex =(CbmVertex*) manager->GetObject("PrimaryVertex.");
	fTracks->GetFromTree();
}

void NicaCbmGlobalEventInterface::Register(Bool_t write) {
	FairRootManager *manager = FairRootManager::Instance();
	manager->Register("PrimaryVertex.","PrimaryVertex.",fVertex,write);
	fTracks->Register(write);
}

void NicaCbmGlobalEventInterface::CopyData(NicaEventInterface* s) {
	NicaCbmGlobalEventInterface *ev  =  (NicaCbmGlobalEventInterface*)s;
	*fVertex =* ev->fVertex;
	fTracks->CopyFrom<CbmGlobalTrack>(ev->fTracks->GetArray());
}

void NicaCbmGlobalEventInterface::Compress(Int_t* map, Int_t map_size) {
	fTracks->Compress(map, map_size);
}

void NicaCbmGlobalEventInterface::CopyAndCompress(NicaEventInterface* s, Int_t* map,
		Int_t map_size) {
	NicaCbmGlobalEventInterface *ev  =  (NicaCbmGlobalEventInterface*)s;
	*fVertex = *ev->fVertex;
	fTracks->CopyCompress<CbmGlobalTrack>(ev->fTracks->GetArray(), map, map_size);
}

void NicaCbmGlobalEventInterface::FillTrackInterface(NicaTrackInterface* track,
		Int_t index) {
	NicaCbmGlobalTrackInterface *cbm_track = (NicaCbmGlobalTrackInterface*)track;
	cbm_track->SetRawTrack(fTracks->UncheckedAt(index));
}

Int_t NicaCbmGlobalEventInterface::GetTotalTrackNo() const {
	return fTracks->GetEntriesFast();
}

TObject* NicaCbmGlobalEventInterface::GetRawTrackPointer(Int_t index) const {
	return fTracks->UncheckedAt(index);
}

TLorentzVector NicaCbmGlobalEventInterface::GetVertexError() const {
	return TLorentzVector(0,0,0,0);
}

TLorentzVector NicaCbmGlobalEventInterface::GetVertex() const {
	return TLorentzVector(fVertex->GetX(),fVertex->GetY(),fVertex->GetZ(),0);
}

NicaCbmGlobalEventInterface::~NicaCbmGlobalEventInterface() {
	if(CanDeleteEvent()){
		if(fVertex) delete fVertex;
		if(fTracks) delete fTracks;
	}
}

