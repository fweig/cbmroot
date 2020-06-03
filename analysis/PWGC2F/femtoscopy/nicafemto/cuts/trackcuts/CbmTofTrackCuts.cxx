/*
 * CbmTofTrackCuts.cxx
 *
 *  Created on: 31 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CbmTofTrackCuts.h"
#include "CbmGlobalTrack.h"
#include "NicaCbmTrack.h"
#include "CbmTofHit.h"

CbmTofTrackCuts::CbmTofTrackCuts():NicaTrackCut(2),
fOneOverC(1.0/29.99792458){
	SetUnitName("#beta [c]",Beta());
	SetUnitName("m^2 [GeV^2/c^4]",M2());
	SetMinMax(0,2,Beta());
	SetMinMax(-10,10,M2());

}

void CbmTofTrackCuts::SetM2Cut(Double_t min, Double_t max) {
	SetMinMax(min,max,M2());
}

void CbmTofTrackCuts::SetBetaCut(Double_t min, Double_t max) {
	SetMinMax(min,max,Beta());
}

Bool_t CbmTofTrackCuts::Pass(NicaTrack *track) {
	NicaCbmTrack *tr = (NicaCbmTrack*)track;
	Double_t l = tr->GetTrackLenght();
	CbmTofHit *h = tr->GetTofTrack();
	if(h==NULL){
		SetValue(2, Beta());
		SetValue(-1000,M2());
		return ForcedUpdate(kFALSE);
	}
	Double_t t = h->GetTime();
	Double_t beta =  l/t/(29.9792458);
	Double_t p =tr->GetMomentum()->P();
	Double_t m2  = p*p*(1./beta/beta-1.);
	SetValue(beta,Beta());
	SetValue(m2,M2());
	return Validate();
}

CbmTofTrackCuts::~CbmTofTrackCuts() {
	// TODO Auto-generated destructor stub
}

CbmTofM2Cut::CbmTofM2Cut() :fOneOverC(1.0/29.99792458){
	SetUnitName("m^2 [GeV^2/c^4]");
}

Bool_t CbmTofM2Cut::Pass(NicaTrack *track) {
	NicaCbmTrack *tr = (NicaCbmTrack*)track;
	Double_t l = tr->GetTrackLenght();
	CbmTofHit *h = tr->GetTofTrack();
	if(h==NULL){
		SetValue(-1000);
		return ForcedUpdate(kFALSE);
	}
	Double_t p =tr->GetMomentum()->P();
	Double_t t = h->GetTime();
	Double_t beta =  l/t/(29.9792458);
	Double_t m2  = p*p*(1./beta/beta-1.);
	SetValue(m2);
	Double_t min = fLowLine[0] + fLowLine[1]*p+fLowLine[2]*p*p;
	Double_t max = fHighLine[0] + fHighLine[1]*p+fHighLine[2]*p*p;
	if(m2>max||m2<min){
		return ForcedUpdate(kFALSE);
	}
	return Validate();
}

CbmTofM2Cut::~CbmTofM2Cut() {
}

CbmTofMassCut::CbmTofMassCut() :fOneOverC(1.0/29.99792458){
	SetUnitName("m [GeV/c]");
}

Bool_t CbmTofMassCut::Pass(NicaTrack *track) {
	NicaCbmTrack *tr = (NicaCbmTrack*)track;
	Double_t l = tr->GetTrackLenght();
	CbmTofHit *h = tr->GetTofTrack();
	if(h==NULL){
		SetValue(-1000);
		return ForcedUpdate(kFALSE);
	}
	Double_t t = h->GetTime();
	Double_t beta =  l/t/(29.9792458);
	Double_t p =tr->GetMomentum()->P();
	Double_t gamma = 1./(TMath::Sqrt(1.-beta*beta));
	Double_t m = p/beta/gamma;
	SetValue(m);
	return Validate();
}

CbmTofMassCut::~CbmTofMassCut() {
}

Bool_t CbmTofM2Cut::Init(Int_t format_id) {
	fInit = kTRUE;
	return FormatInhertis("NicaCbmEvent", format_id);
}

CbmTofM2Cut::CbmTofM2Cut(const CbmTofM2Cut &other) :
		NicaTrackToFMass2Cut(other),
		fOneOverC(1.0/29.99792458){
	for(int i=0;i<3;i++){
		fLowLine[i] = other.fLowLine[i];
		fHighLine[i] = other.fHighLine[i];
	}
}
