/*
 * CbmSailorCowboyCut.cxx
 *
 *  Created on: 9 cze 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CbmSailorCowboyCut.h"

#include "CbmHelix.h"
#include "CbmStsExitSepCut.h"
#include "NicaExpEvent.h"
#include "NicaTwoTrack.h"


CbmSailorCowboyCut::CbmSailorCowboyCut() : NicaTwoTrackCut(1) {
  SetUnitName("Sail Cow [Au]", 0);
  SetMinMax(0, 1);
}

CbmSailorCowboyCut::~CbmSailorCowboyCut() {}

Bool_t CbmSailorCowboyCut::Init(Int_t task_id) {
  if (FormatInhertis("NicaCbmEvent", task_id)
      || FormatInhertis("NicaCbmGlobalMCEvent", task_id)) {
    return kTRUE;
  }
  return kFALSE;
}

Bool_t CbmSailorCowboyCut::Pass(NicaTwoTrack* pair) {
  NicaCbmTrack* tr1 = static_cast<NicaCbmTrack*>(pair->GetTrack1());
  NicaCbmTrack* tr2 = static_cast<NicaCbmTrack*>(pair->GetTrack2());
  TLorentzVector* Pos1 =
    static_cast<NicaExpEvent*>(tr1->GetEvent())->GetVertex();
  TLorentzVector* Pos2 =
    static_cast<NicaExpEvent*>(tr2->GetEvent())->GetVertex();

  CbmHelix* h1 = tr1->GetHelix();
  CbmHelix* h2 = tr2->GetHelix();
  TVector3 pos1[4], pos2[4];
  pos1[0]        = h1->Eval(Pos1->Z());
  pos2[0]        = h2->Eval(Pos2->Z());
  pos1[1]        = h1->Eval(Pos1->Z() + 25.0);
  pos2[1]        = h2->Eval(Pos2->Z() + 25.0);
  pos1[2]        = h1->Eval(Pos1->Z() + 50.0);
  pos2[2]        = h2->Eval(Pos2->Z() + 50.0);
  pos1[3]        = h1->Eval(Pos1->Z() + 75.0);
  pos2[3]        = h2->Eval(Pos2->Z() + 75.0);
  Double_t minXY = -1000.0;
  for (int i = 0; i < 4; i++) {
    Double_t dx = (pos1[i].X() - Pos1->X()) - (pos2[i].X() - Pos2->X());
    Double_t dy = (pos1[i].Y() - Pos1->Y()) - (pos2[i].Y() - Pos2->Y());
    Double_t dR = dx * dx + dy * dy;
    if (dR < minXY) {  //distance smaller than previous  - cowboy
      SetValue(Cowboy());
      return Validate();
    } else {
      minXY = dR;
    }
  }
  SetValue(Sailor());
  return Validate();
}
