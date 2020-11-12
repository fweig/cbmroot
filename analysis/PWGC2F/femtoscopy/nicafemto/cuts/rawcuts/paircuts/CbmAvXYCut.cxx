/*
 * CbmAvXYCut.cpp
 *
 *  Created on: 15 cze 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CbmAvXYCut.h"

#include "CbmHelix.h"
#include "NicaExpEvent.h"
#include "NicaTwoTrack.h"

#include <TLorentzVector.h>
#include <TVector3.h>

CbmAvXYCut::CbmAvXYCut() : NicaTwoTrackCut(2) {
  SetUnitName("X_{av} [cm]", 0);
  SetUnitName("Y_{av} [cm]", 1);
  SetMinMax(-90, 90, 0);
  SetMinMax(-90, 90, 1);
}

Bool_t CbmAvXYCut::Init(Int_t task_id) {
  if (FormatInhertis("NicaCbmEvent", task_id)
      || FormatInhertis("NicaCbmGlobalMCEvent", task_id)) {
    return kTRUE;
  }
  return kFALSE;
}

Bool_t CbmAvXYCut::Pass(NicaTwoTrack* pair) {
  NicaCbmTrack* tr1 = static_cast<NicaCbmTrack*>(pair->GetTrack1());
  NicaCbmTrack* tr2 = static_cast<NicaCbmTrack*>(pair->GetTrack2());
  TLorentzVector* Pos1 =
    static_cast<NicaExpEvent*>(tr1->GetEvent())->GetVertex();
  TLorentzVector* Pos2 =
    static_cast<NicaExpEvent*>(tr2->GetEvent())->GetVertex();

  CbmHelix* h1  = tr1->GetHelix();
  CbmHelix* h2  = tr2->GetHelix();
  TVector3 pos1 = h1->Eval(50.1 + Pos1->Z());
  TVector3 pos2 = h2->Eval(50.1 + Pos2->Z());
  pos1.SetX(pos1.X() - Pos1->X());
  pos1.SetY(pos1.Y() - Pos1->Y());
  pos2.SetX(pos2.X() - Pos2->X());
  pos2.SetY(pos2.Y() - Pos2->Y());

  TVector3 av = pos1 + pos2;
  SetValue(av.X() * 0.5, 0);
  SetValue(av.Y() * 0.5, 1);
  return Validate();
}

CbmAvXYCut::~CbmAvXYCut() {}
