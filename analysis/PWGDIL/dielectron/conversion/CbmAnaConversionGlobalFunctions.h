/**
 * @author S. Reinecke <reinecke@uni-wuppertal.de>
 * @since 2016
 **/

#ifndef CBM_ANA_CONVERSION_GLOBAL_FUNCTIONS
#define CBM_ANA_CONVERSION_GLOBAL_FUNCTIONS

#include "CbmMCTrack.h"
#include "CbmGlobalTrack.h"
#include "CbmRichRing.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "TClonesArray.h"
#include "CbmRichElectronIdAnn.h"

#define M2E 2.6112004954086e-7

class CbmAnaConversionGlobalFunctions{
public:

	/*
	 * Calculate ANN response for a given globaltrack index and momentum
	 */
	static Double_t ElectronANNvalue(Int_t globalTrackIndex, Double_t momentum)
	{
		FairRootManager *ioman = FairRootManager::Instance();
		if (NULL == ioman) return -2;
		TClonesArray *fGlobalTracks = (TClonesArray*) ioman->GetObject("GlobalTrack");
		if (NULL == fGlobalTracks) return -2;
		TClonesArray *fRichRings = (TClonesArray*) ioman->GetObject("RichRing");
		if (NULL == fRichRings) return -2;
		
		//if (NULL == fGlobalTracks || NULL == fRichRings) return -2;
		//CbmGlobalTrack* globalTrack = (CbmGlobalTrack*) fGlobalTracks->At(globalTrackIndex);
		const CbmGlobalTrack* globalTrack = static_cast<const CbmGlobalTrack*>(fGlobalTracks->At(globalTrackIndex));
		Int_t richId = globalTrack->GetRichRingIndex();
		if (richId < 0) return -2;
		CbmRichRing* ring = static_cast<CbmRichRing*> (fRichRings->At(richId));
		if (NULL == ring) return -2;

		Double_t ann = CbmRichElectronIdAnn::GetInstance().CalculateAnnValue(globalTrackIndex, momentum);
		return ann;
	}

	/*
	 * Checks, whether a track is an electron based on ANN output for a given globaltrack index and momentum, and for a given ANNcut value
	 */
	static Double_t IsRICHElectronANN(Int_t globalTrackIndex, Double_t momentum, Double_t ANNcut)
	{
		FairRootManager *ioman = FairRootManager::Instance();
		if (NULL == ioman) return -2;
		TClonesArray *fGlobalTracks = (TClonesArray*) ioman->GetObject("GlobalTrack");
		if (NULL == fGlobalTracks) return -2;
		TClonesArray *fRichRings = (TClonesArray*) ioman->GetObject("RichRing");
		if (NULL == fRichRings) return -2;

		
		//if (NULL == fGlobalTracks || NULL == fRichRings) return -2;
		//CbmGlobalTrack* globalTrack = (CbmGlobalTrack*) fGlobalTracks->At(globalTrackIndex);
		const CbmGlobalTrack* globalTrack = static_cast<const CbmGlobalTrack*>(fGlobalTracks->At(globalTrackIndex));
		Int_t richId = globalTrack->GetRichRingIndex();
		if (richId < 0) return -2;
		CbmRichRing* ring = static_cast<CbmRichRing*> (fRichRings->At(richId));
		if (NULL == ring) return -2;

		Double_t ann = CbmRichElectronIdAnn::GetInstance().CalculateAnnValue(globalTrackIndex, momentum);
		if(ann > ANNcut) return true;
		else return false;
	}




	static Double_t OpeningAngleBetweenGamma(const TVector3 part11, const TVector3 part12, const TVector3 part21, const TVector3 part22)
	{
		Double_t openingAngle = 0;
	
		Double_t energy11 = TMath::Sqrt(part11.Mag2() + M2E);
		TLorentzVector lorVec11(part11, energy11);

		Double_t energy12 = TMath::Sqrt(part12.Mag2() + M2E);
		TLorentzVector lorVec12(part12, energy12);

		Double_t energy21 = TMath::Sqrt(part21.Mag2() + M2E);
		TLorentzVector lorVec21(part21, energy21);

		Double_t energy22 = TMath::Sqrt(part22.Mag2() + M2E);
		TLorentzVector lorVec22(part22, energy22);
		
		TLorentzVector gamma1 = lorVec11 + lorVec12;
		TLorentzVector gamma2 = lorVec21 + lorVec22;
		
		Double_t angle = gamma1.Angle(gamma2.Vect()); 
		openingAngle = 180.*angle/TMath::Pi();

		return openingAngle;
	}


};

#endif
