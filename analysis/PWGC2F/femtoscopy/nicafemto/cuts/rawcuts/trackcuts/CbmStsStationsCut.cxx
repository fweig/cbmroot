/*
 * CbmStsStationsCut.cxx
 *
 *  Created on: 7 sie 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CbmStsStationsCut.h"

#include "CbmStsTrack.h"
#include "NicaCbmTrack.h"

CbmStsStationsCut::CbmStsStationsCut() : NicaTrackCut(1) {
  SetUnitName("N_{STS stations}");
}

Bool_t CbmStsStationsCut::Init(Int_t format_id) {
  fInit = kTRUE;
  return FormatInhertis("NicaCbmEvent", format_id);
}

Bool_t CbmStsStationsCut::Pass(NicaTrack* tr) {
  NicaCbmTrack* track = (NicaCbmTrack*) tr;
  CbmStsTrack* sts    = track->GetStsTrack();
  if (!sts) {
    SetValue(0);
    return ForcedUpdate(kFALSE);
  }
  //	SetValue(sts->GetStationsNo());
  return Validate();
}

CbmStsStationsCut::~CbmStsStationsCut() {}
