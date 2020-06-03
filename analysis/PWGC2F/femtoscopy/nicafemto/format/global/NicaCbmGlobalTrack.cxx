/*
 * NicaCbmTrack.cxx
 *
 *  Created on: 3 gru 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "NicaCbmGlobalTrack.h"
#include "NicaCbmGlobalEventInterface.h"
#include "NicaExpEvent.h"

NicaCbmGlobalTrack::NicaCbmGlobalTrack() :
	fFirstPoint(new TVector3(0,0,0)),
	fLastPoint(new TVector3(0,0,0)){
}

NicaCbmGlobalTrack::NicaCbmGlobalTrack(const NicaCbmGlobalTrack& other) :NicaExpTrack(other){
	fFirstPoint = new TVector3(*other.fFirstPoint);
	fLastPoint = new TVector3(*other.fLastPoint);
	fHelix = other.fHelix;
}

NicaCbmGlobalTrack& NicaCbmGlobalTrack::operator =(const NicaCbmGlobalTrack& other) {
	if(this==&other) return *this;
	NicaExpTrack::operator=(other);
	*fFirstPoint = *other.fFirstPoint;
	*fLastPoint  = *other.fLastPoint;
	fHelix = other.fHelix;
	return *this;
}

void NicaCbmGlobalTrack::Update(CbmGlobalTrack* track,NicaCbmGlobalEventInterface */*interface*/) {
	SetChi2(track->GetChi2());
	SetTrackLenght(track->GetLength());
	const FairTrackParam *par = track->GetParamFirst();
	fFirstPoint->SetXYZ(par->GetX(),par->GetY(),par->GetZ());
	const FairTrackParam *par2 = track->GetParamLast();
	fLastPoint->SetXYZ(par2->GetX(),par2->GetY(),par2->GetZ());
	const CbmTrackParam *track_param = track->GetParamVertex();
	GetMomentum()->SetXYZM(track_param->GetPx(), track_param->GetPy(), track_param->GetPz(),0);

	const NicaExpEvent *ev = (NicaExpEvent*)GetEvent();
	fHelix.Build(track);
	GetDCA()->SetXYZ(
			track_param->GetX()-ev->GetVertex()->X(),
			track_param->GetY()-ev->GetVertex()->Y(),
			track_param->GetZ()-ev->GetVertex()->Z());
	Double_t qp = track_param->GetQp();
	if(qp>0)
		SetCharge(1);
	else
		SetCharge(-1);
}

void NicaCbmGlobalTrack::CopyData(NicaTrack* other){
	NicaExpTrack::CopyData(other);
	NicaCbmGlobalTrack *track = (NicaCbmGlobalTrack*)other;
	*fFirstPoint = *track->fFirstPoint;
	*fLastPoint  = *track->fLastPoint;
	fHelix = track->fHelix;
}

NicaCbmGlobalTrack::~NicaCbmGlobalTrack() {
	delete fFirstPoint;
	delete fLastPoint;
}

