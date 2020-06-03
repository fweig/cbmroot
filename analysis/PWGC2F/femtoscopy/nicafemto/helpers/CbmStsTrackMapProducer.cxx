/*
 * CbmStsTrackMapProducer.cxx
 *
 *  Created on: 7 sie 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CbmStsTrackMapProducer.h"
#include "CbmStsTrack.h"
#include "CbmStsHit.h"
#include <bitset>
#include <iostream>
CbmStsTrackMapProducer::CbmStsTrackMapProducer() {
	fStsHits = new NicaTrackClones("CbmStsHit","StsHit","STS");
	fStsTracks = new NicaTrackClones("CbmStsTrack","StsTrack","STS");
}

InitStatus CbmStsTrackMapProducer::Init() {
	fStsHits->GetFromTree();
	fStsTracks->GetFromTree();
	return kSUCCESS;
}

void CbmStsTrackMapProducer::Exec(Option_t* /*opt*/) {
	for(int i=0;i<fStsTracks->GetEntriesFast();i++){
		CbmStsTrack *track = (CbmStsTrack*)fStsTracks->UncheckedAt(i);
		Int_t hits = track->GetNofStsHits();
		UShort_t bitmap = 0;
		Int_t stations[8]={0};
		for(int j=0;j<hits;j++){
			CbmStsHit *hit = (CbmStsHit*)fStsHits->UncheckedAt(track->GetHitIndex(j));
			Int_t station_no = GetStation(hit);
			stations[station_no]++;
		}
		for(int j=0;j<8;j++){
			switch(stations[j]){
			case 0://do nothing
				break;
			case 1://one hit per station
				SETBIT(bitmap,j*2);
				break;
			default://at least two hits in station
				std::cout<<"MULTIPLE HIT ! "<<std::endl;
				SETBIT(bitmap,j*2);
				SETBIT(bitmap,j*2+1);
				break;
			}
		}
	//	track->SetStationMap(bitmap);
		std::cout<<std::bitset<16>(bitmap)<<std::endl;
	}
}

Int_t CbmStsTrackMapProducer::GetStation(CbmStsHit *hit) const {
	Int_t z = hit->GetZ();
	if(z<35) return 0;
	if(z<45) return 1;
	if(z<55) return 2;
	if(z<65) return 3;
	if(z<75) return 4;
	if(z<85) return 5;
	if(z<95) return 6;
	if(z<105) return 7;
	if(z<115) return 8;
	 z = hit->GetZ()-25;


	return (z-z%10)/10;
}

CbmStsTrackMapProducer::~CbmStsTrackMapProducer() {
	// TODO Auto-generated destructor stub
}

