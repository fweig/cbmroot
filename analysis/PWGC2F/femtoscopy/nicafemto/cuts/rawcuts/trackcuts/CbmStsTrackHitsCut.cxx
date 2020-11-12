/*
 * CbmStsTrackHitsCut.cxx
 *
 *  Created on: 17 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CbmStsTrackHitsCut.h"

#include "CbmStsTrack.h"

CbmStsTrackHitsCut::CbmStsTrackHitsCut() : NicaTrackCut(3) {
  SetUnitName("STS hits [N]", Sts());
  SetUnitName("MV Dhits [N]", Mvd());
  SetUnitName("STS+MVD hits [N]", StsMvd());
  SetMinMax(0, 1000, Sts());
  SetMinMax(0, 1000, Mvd());
  SetMinMax(0, 1000, StsMvd());
}

Bool_t CbmStsTrackHitsCut::Pass(NicaTrack* track) {
  NicaCbmTrack* tr    = (NicaCbmTrack*) track;
  CbmStsTrack* sts_tr = tr->GetStsTrack();
  if (sts_tr == NULL) {
    SetValue(0, Sts());
    SetValue(0, Mvd());
    SetValue(0, StsMvd());
  } else {
    SetValue(sts_tr->GetNofStsHits(), Sts());
    SetValue(sts_tr->GetNofMvdHits(), Mvd());
    SetValue(sts_tr->GetNofHits(), StsMvd());
  }
  return Validate();
}

Bool_t CbmStsTrackHitsCut::Init(Int_t format_id) {
  fInit = kTRUE;
  if (FormatInhertis("NicaCbmEvent", format_id)) { return kTRUE; }
  return kFALSE;
}

CbmStsTrackHitsCut::~CbmStsTrackHitsCut() {}
