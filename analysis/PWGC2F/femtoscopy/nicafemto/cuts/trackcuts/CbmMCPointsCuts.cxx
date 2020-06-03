/*
 * CbmMCPointsCuts.cxx
 *
 *  Created on: 18 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CbmMCPointsCuts.h"
#include "NicaCbmMCTrack.h"

NicaCbmMCPointsCut::NicaCbmMCPointsCut(Int_t params):NicaTrackCut(params) {
}

Bool_t NicaCbmMCPointsCut::Init(Int_t format_id) {
	if(FormatInhertis("NicaCbmMCEvent", format_id)){
		fInit = kTRUE;
		return kTRUE;
	}
	return kFALSE;
}

NicaCbmStsPointsCut::NicaCbmStsPointsCut() :NicaCbmMCPointsCut(1){
	SetUnitName("N_{STS Points} [N]",0);
}

Bool_t NicaCbmStsPointsCut::Pass(NicaTrack *track) {
	NicaCbmMCTrack *tr = (NicaCbmMCTrack*)track;
	SetValue(tr->GetNPoints(ECbmModuleId::kSts));
	return Validate();
}

NicaCbmMvdPointsCut::NicaCbmMvdPointsCut() :NicaCbmMCPointsCut(1) {
	SetUnitName("N_{MVD Points} [N]",0);
}

Bool_t NicaCbmMvdPointsCut::Pass(NicaTrack *track) {
	NicaCbmMCTrack *tr = (NicaCbmMCTrack*)track;
	SetValue(tr->GetNPoints(ECbmModuleId::kMvd));
	return Validate();
}

NicaCbmRichPointsCut::NicaCbmRichPointsCut()  :NicaCbmMCPointsCut(1){
	SetUnitName("N_{RICH Points} [N]",0);
}

Bool_t NicaCbmRichPointsCut::Pass(NicaTrack *track) {
	NicaCbmMCTrack *tr = (NicaCbmMCTrack*)track;
	SetValue(tr->GetNPoints(ECbmModuleId::kRich));
	return Validate();
}

NicaCbmMuchPointsCut::NicaCbmMuchPointsCut() :NicaCbmMCPointsCut(1) {
	SetUnitName("N_{MUCH Points} [N]",0);
}

Bool_t NicaCbmMuchPointsCut::Pass(NicaTrack *track) {
	NicaCbmMCTrack *tr = (NicaCbmMCTrack*)track;
	SetValue(tr->GetNPoints(ECbmModuleId::kMuch));
	return Validate();
}

NicaCbmTrdPointsCut::NicaCbmTrdPointsCut()  :NicaCbmMCPointsCut(1){
	SetUnitName("N_{TRD Points} [N]",0);
}

Bool_t NicaCbmTrdPointsCut::Pass(NicaTrack *track) {
	NicaCbmMCTrack *tr = (NicaCbmMCTrack*)track;
	SetValue(tr->GetNPoints(ECbmModuleId::kTrd));
	return Validate();
}

NicaCbmTofPointsCut::NicaCbmTofPointsCut()  :NicaCbmMCPointsCut(1){
	SetUnitName("N_{TOFPoints} [N]",0);
}

Bool_t NicaCbmTofPointsCut::Pass(NicaTrack *track) {
	NicaCbmMCTrack *tr = (NicaCbmMCTrack*)track;
	SetValue(tr->GetNPoints(ECbmModuleId::kTof));
	return Validate();
}

NicaCbmEcalPointsCut::NicaCbmEcalPointsCut()  :NicaCbmMCPointsCut(1){
	SetUnitName("N_{ECALPoints} [N]",0);
}

Bool_t NicaCbmEcalPointsCut::Pass(NicaTrack *track) {
	NicaCbmMCTrack *tr = (NicaCbmMCTrack*)track;
	SetValue(tr->GetNPoints(ECbmModuleId::kEcal));
	return Validate();
}

NicaCbmPsdPointsCut::NicaCbmPsdPointsCut()  :NicaCbmMCPointsCut(1){
	SetUnitName("N_{PSD Points} [N]",0);
}

Bool_t NicaCbmPsdPointsCut::Pass(NicaTrack *track) {
	NicaCbmMCTrack *tr = (NicaCbmMCTrack*)track;
	SetValue(tr->GetNPoints(ECbmModuleId::kPsd));
	return Validate();
}
