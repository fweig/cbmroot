
#ifndef CBM_ANA_JPSI_UTILS_H
#define CBM_ANA_JPSI_UTILS_H

#include "CbmAnaJpsiCandidate.h"

#include "CbmStsTrack.h"
#include "CbmMCTrack.h"
#include "CbmKFVertex.h"
#include "CbmL1PFFitter.h"
#include "L1Field.h"

#include "TDatabasePDG.h"
#include "TMCProcess.h"

#include <vector>

class CbmAnaJpsiUtils{
public:

	/*
	 * Calculates and set track parameters to CbmAnaJpsiCandidate.
	 * The following parameters are set: fChi2sts, fChi2Prim, fPosition, fMomentum, fMass, fCharge, fEnergy, fRapidity
	 */
	static void CalculateAndSetTrackParamsToCandidate(
	    CbmAnaJpsiCandidate* cand,
		CbmStsTrack* stsTrack,
		CbmKFVertex& kfVertex)
	{
		CbmL1PFFitter fPFFitter;
		std::vector<CbmStsTrack> stsTracks;
		stsTracks.resize(1);
		stsTracks[0] = *stsTrack;
		std::vector<L1FieldRegion> vField;
		std::vector<float> chiPrim;
	    fPFFitter.GetChiToVertex(stsTracks, vField, chiPrim, kfVertex, 3e6);
	    cand->fChi2sts = stsTracks[0].GetChiSq() / stsTracks[0].GetNDF();
	    cand->fChi2Prim = chiPrim[0];

		const FairTrackParam* vtxTrack = stsTracks[0].GetParamFirst();

		vtxTrack->Position(cand->fPosition);
		vtxTrack->Momentum(cand->fMomentum);

		cand->fMass = TDatabasePDG::Instance()->GetParticle(11)->Mass();
		cand->fCharge = (vtxTrack->GetQp() > 0) ?1 :-1;
		cand->fEnergy = sqrt(cand->fMomentum.Mag2() + cand->fMass * cand->fMass);
		cand->fRapidity = 0.5*TMath::Log((cand->fEnergy + cand->fMomentum.Z()) / (cand->fEnergy - cand->fMomentum.Z()));
	}

	/*
	 * \brief Return true if MC track is signal primary electron.
	 */
	static Bool_t IsMcSignalElectron(
			CbmMCTrack* mctrack)
	{
		if (mctrack == NULL) return false;
		Int_t pdg = TMath::Abs(mctrack->GetPdgCode());
		if (mctrack->GetGeantProcessId() == kPPrimary && pdg == 11) return true;
		return false;
	}

    /*
     * \brief Return true if MC track is electron from gamma conversion.
     */
	static Bool_t IsMcGammaElectron(
			CbmMCTrack* mctrack,
			TClonesArray* mcTracks)
	{
		if (mctrack == NULL) return false;
		Int_t pdg = TMath::Abs(mctrack->GetPdgCode());
		if (pdg != 11) return false;
		Int_t motherId = mctrack->GetMotherId();
		if (motherId < 0){
			return false;
		} else {
			CbmMCTrack* mct1 = static_cast<CbmMCTrack*>(mcTracks->At(motherId));
		    Int_t motherPdg = mct1->GetPdgCode();
		    if (mct1 != NULL && motherPdg == 22 && pdg == 11){
		    	return true;
		    }
		}
		return false;
	}

    /*
     * \brief Return true if MC track is electron from Pi0 dalitz decay.
     */
	static Bool_t IsMcPi0Electron(
			CbmMCTrack* mctrack,
			TClonesArray* mcTracks)
	{
		if (mctrack == NULL) return false;
		Int_t pdg = TMath::Abs(mctrack->GetPdgCode());
		if (pdg != 11) return false;
		Int_t motherId = mctrack->GetMotherId();
		if (motherId < 0){
			return false;
		} else {
			CbmMCTrack* mct1 = static_cast<CbmMCTrack*>(mcTracks->At(motherId));
		    Int_t motherPdg = mct1->GetPdgCode();
		    if (mct1 != NULL && motherPdg == 111 && pdg == 11){
		    	return true;
		    }
		}
		return false;
	}
};

#endif
