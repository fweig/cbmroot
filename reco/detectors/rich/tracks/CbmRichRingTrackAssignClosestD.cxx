/**
* \file CbmRichRingTrackAssignClosestD.cxx
*
* \author Claudia Hoehne and Semen Lebedev
* \date 2007
**/

#include "CbmRichRingTrackAssignClosestD.h"

#include "CbmRichRing.h"

#include "CbmMCTrack.h"
#include "FairRootManager.h"
#include "FairTrackParam.h"
#include "CbmGlobalTrack.h"
#include "CbmTrdTrack.h"

#include "TClonesArray.h"

#include <iostream>
#include <algorithm>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

CbmRichRingTrackAssignClosestD::CbmRichRingTrackAssignClosestD():
   fGlobalTracks(NULL),
   fTrdTracks(NULL),
   fTrdAnnCut(-0.5),
   fUseTrd(false),
   fAlgorithmType(RingTrack)
{

}

CbmRichRingTrackAssignClosestD::~CbmRichRingTrackAssignClosestD()
{
}

void CbmRichRingTrackAssignClosestD::Init()
{
	FairRootManager* ioman = FairRootManager::Instance();
	if (NULL == ioman) {Fatal("CbmRichRingTrackAssignClosestD::Init", "RootManager not instantised!");}

	fGlobalTracks = (TClonesArray*) ioman->GetObject("GlobalTrack");
	if (NULL == fGlobalTracks) {Fatal("CbmRichRingTrackAssignClosestD::Init", "No GlobalTrack array!");}

	fTrdTracks = (TClonesArray*) ioman->GetObject("TrdTrack");
	//if (NULL == fTrdTracks) {Fatal("CbmRichRingTrackAssignClosestD::Init", "No TrdTrack array!");}
}

void CbmRichRingTrackAssignClosestD::DoAssign(
      TClonesArray* rings,
      TClonesArray* richProj)
{
	if (fAlgorithmType == RingTrack) {
		DoAssignRingTrack(rings, richProj);
	} // RingTrack algorithm

	else if (fAlgorithmType == TrackRing) {
		DoAssignTrackRing(rings, richProj);
	} // TrackRing

	else if (fAlgorithmType == Combined) {
		DoAssignRingTrack(rings, richProj);
		DoAssignTrackRing(rings, richProj);
	} // Combined
}

void CbmRichRingTrackAssignClosestD::DoAssignRingTrack(
      TClonesArray* rings,
      TClonesArray* richProj)
{
    Int_t nofTracks = richProj->GetEntriesFast();
	Int_t nofRings = rings->GetEntriesFast();
	vector<Int_t> trackIndex;
	vector<Double_t> trackDist;
	trackIndex.resize(nofRings);
	trackDist.resize(nofRings);
	for (UInt_t i = 0; i < trackIndex.size(); i++){
		trackIndex[i] = -1;
		trackDist[i] = 999.;
	}

	for (Int_t iIter = 0; iIter < 4; iIter++){
		for (Int_t iRing=0; iRing < nofRings; iRing++) {
			if (trackIndex[iRing] != -1) continue;
			CbmRichRing* pRing = (CbmRichRing*)rings->At(iRing);
			if (NULL == pRing) continue;
			if (pRing->GetNofHits() < fMinNofHitsInRing) continue;

			Double_t xRing = pRing->GetCenterX();
			Double_t yRing = pRing->GetCenterY();
			Double_t rMin = 999.;
			Int_t iTrackMin = -1;

			for (Int_t iTrack=0; iTrack < nofTracks; iTrack++) {
				vector<Int_t>::iterator it = find(trackIndex.begin(), trackIndex.end(), iTrack);
				if (it != trackIndex.end()) continue;

				FairTrackParam* pTrack = (FairTrackParam*)richProj->At(iTrack);
				Double_t xTrack = pTrack->GetX();
				Double_t yTrack = pTrack->GetY();
				// no projection onto the photodetector plane
				if (xTrack == 0 && yTrack == 0) continue;

				if (fUseTrd && fTrdTracks != NULL && !IsTrdElectron(iTrack)) continue;

				Double_t dist = TMath::Sqrt( (xRing-xTrack)*(xRing-xTrack) + (yRing-yTrack)*(yRing-yTrack) );

				if (dist < rMin) {
					rMin = dist;
					iTrackMin = iTrack;
				}
			} // loop tracks
			trackIndex[iRing] = iTrackMin;
			trackDist[iRing] = rMin;
		}//loop rings

		for (UInt_t i1 = 0; i1 < trackIndex.size(); i1++){
			for (UInt_t i2 = 0; i2 < trackIndex.size(); i2++){
				if (i1 == i2) continue;
				if (trackIndex[i1] == trackIndex[i2] && trackIndex[i1] != -1){
					if (trackDist[i1] >= trackDist[i2]){
						trackDist[i1] = 999.;
						trackIndex[i1] = -1;
					}else{
						trackDist[i2] = 999.;
						trackIndex[i2] = -1;
					}
				}
			}
		}
	}//iIter

	// fill global tracks
	for (UInt_t i = 0; i < trackIndex.size(); i++){
//		CbmRichRing* pRing = (CbmRichRing*)rings->At(i);
	   // cout << "trackIndex[i]:" << trackIndex[i] << " trackDist[i]:" << trackDist[i] << " r:" << pRing->GetRadius() << " x:" << pRing->GetCenterX() << " y:" << pRing->GetCenterY()<< endl;
		if (trackIndex[i] == -1) continue;
		CbmGlobalTrack* gTrack = (CbmGlobalTrack*) fGlobalTracks->At(trackIndex[i]);
		gTrack->SetRichRingIndex(i);
	}
}

void CbmRichRingTrackAssignClosestD::DoAssignTrackRing(
      TClonesArray* rings,
      TClonesArray* richProj)
{
    Int_t nofTracks = richProj->GetEntriesFast();
	Int_t nofRings = rings->GetEntriesFast();

	for (Int_t iTrack=0; iTrack < nofTracks; iTrack++) {
		CbmGlobalTrack* gTrack = (CbmGlobalTrack*) fGlobalTracks->At(iTrack);
		// track already has rich segment
		if (gTrack == NULL || gTrack->GetRichRingIndex() >= 0) continue;

		FairTrackParam* pTrack = (FairTrackParam*)richProj->At(iTrack);
		Double_t xTrack = pTrack->GetX();
		Double_t yTrack = pTrack->GetY();
		if (xTrack == 0 && yTrack == 0) continue;
		if (fUseTrd && fTrdTracks != NULL && !IsTrdElectron(iTrack)) continue;
		Double_t rMin = 999.;
		Int_t iRingMin = -1;
		for (Int_t iRing=0; iRing < nofRings; iRing++) {
			CbmRichRing* pRing = (CbmRichRing*)rings->At(iRing);
			if (NULL == pRing) continue;
			if (pRing->GetNofHits() < fMinNofHitsInRing) continue;

			Double_t xRing = pRing->GetCenterX();
			Double_t yRing = pRing->GetCenterY();
			Double_t dist = TMath::Sqrt( (xRing-xTrack)*(xRing-xTrack) + (yRing-yTrack)*(yRing-yTrack) );
			if (dist < rMin) {
				rMin = dist;
				iRingMin = iRing;
			}
		} // loop rings
		if (iRingMin < 0) continue;
//		CbmRichRing* pRing = (CbmRichRing*)rings->At(iRingMin);
		gTrack->SetRichRingIndex(iRingMin);
	}//loop tracks
}

Bool_t CbmRichRingTrackAssignClosestD::IsTrdElectron(
      Int_t iTrack)
{
	CbmGlobalTrack* gTrack = (CbmGlobalTrack*) fGlobalTracks->At(iTrack);
	Int_t trdIndex = gTrack->GetTrdTrackIndex();
	if (trdIndex == -1) return false;
	CbmTrdTrack* trdTrack = (CbmTrdTrack*)fTrdTracks->At(trdIndex);
   if (NULL == trdTrack)return false;

   if (trdTrack->GetPidANN() > fTrdAnnCut) {
    	return true;
   }

   return false;
}
