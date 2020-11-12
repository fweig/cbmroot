/*
 * CbmStsExitSepCut.cxx
 *
 *  Created on: 23 mar 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CbmStsExitSepCut.h"

#include "CbmHbtTrack.h"
#include "CbmHelix.h"
#include "NicaCbmATTrack.h"
#include "NicaExpEvent.h"
#include "NicaTwoTrack.h"

#include <TLorentzVector.h>
#include <TVector3.h>

CbmStsExitSepCut::CbmStsExitSepCut()
  : NicaTwoTrackCut(1), fDataType(NicaCbmFormats::ECbmFormatType::kUnknown) {
  SetUnitName("StsExitSep [cm]");
  SetMinMax(0, 1E+5);
}

Bool_t CbmStsExitSepCut::Pass(NicaTwoTrack* pair) {
  CbmHelix *h1 = nullptr, *h2 = nullptr;
  TLorentzVector *Pos1 = nullptr, *Pos2 = nullptr;
  Double_t dist = 0;
  switch (fDataType) {
    case NicaCbmFormats::ECbmFormatType::kCbmRoot: {
      NicaCbmTrack* tr1 = static_cast<NicaCbmTrack*>(pair->GetTrack1());
      NicaCbmTrack* tr2 = static_cast<NicaCbmTrack*>(pair->GetTrack2());
      Pos1 = static_cast<NicaExpEvent*>(tr1->GetEvent())->GetVertex();
      Pos2 = static_cast<NicaExpEvent*>(tr2->GetEvent())->GetVertex();

      h1            = tr1->GetHelix();
      h2            = tr2->GetHelix();
      TVector3 pos1 = h1->Eval(101.1 + Pos1->Z());
      TVector3 pos2 = h2->Eval(101.1 + Pos2->Z());
      pos1.SetX(pos1.X() - Pos1->X());
      pos1.SetY(pos1.Y() - Pos1->Y());
      pos2.SetX(pos2.X() - Pos2->X());
      pos2.SetY(pos2.Y() - Pos2->Y());
      dist = (pos1 - pos2).Mag();
    } break;
    case NicaCbmFormats::ECbmFormatType::kAnaTree: {
      NicaCbmATTrack* tr1 = static_cast<NicaCbmATTrack*>(pair->GetTrack1());
      NicaCbmATTrack* tr2 = static_cast<NicaCbmATTrack*>(pair->GetTrack2());
      Pos1 = static_cast<NicaExpEvent*>(tr1->GetEvent())->GetVertex();
      Pos2 = static_cast<NicaExpEvent*>(tr2->GetEvent())->GetVertex();

      h1            = tr1->GetHelix();
      h2            = tr2->GetHelix();
      TVector3 pos1 = h1->Eval(101.1 + Pos1->Z());
      TVector3 pos2 = h2->Eval(101.1 + Pos2->Z());
      pos1.SetX(pos1.X() - Pos1->X());
      pos1.SetY(pos1.Y() - Pos1->Y());
      pos2.SetX(pos2.X() - Pos2->X());
      pos2.SetY(pos2.Y() - Pos2->Y());
      dist = (pos1 - pos2).Mag();
    } break;
    case NicaCbmFormats::ECbmFormatType::kHbt: {
      CbmHbtTrack* tr1 = static_cast<CbmHbtTrack*>(pair->GetTrack1());
      CbmHbtTrack* tr2 = static_cast<CbmHbtTrack*>(pair->GetTrack2());
      tr1->CalculatePosAt(101.0);
      tr2->CalculatePosAt(101.0);
      dist = (*tr1->GetPosAtR() - *tr2->GetPosAtR()).Mag();
    } break;
    case NicaCbmFormats::ECbmFormatType::kUnknown: {

    } break;
  }
  SetValue(dist);
  return Validate();
}

Bool_t CbmStsExitSepCut::Init(Int_t task_id) {
  fDataType =
    NicaCbmFormats::GetFormatType(task_id, ENicaFormatDepth::kBuffered);
  if (fDataType == NicaCbmFormats::ECbmFormatType::kUnknown) return kFALSE;
  return kTRUE;
}

CbmStsExitSepCut::~CbmStsExitSepCut() {}
