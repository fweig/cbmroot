/*
 * CbmHbtTrack.cxx
 *
 *  Created on: 21 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CbmHbtTrack.h"

#include "CbmHbtEvent.h"
#include "NicaCbmATTrack.h"

CbmHbtTrack::CbmHbtTrack()
  : NicaExpTrack()
  , fPosAt(new TVector3())
  , fMomAt(new TVector3())
  , fHelix(new CbmHelix())
  , fRPos(0)
  , fRMom(0) {}

CbmHbtTrack::~CbmHbtTrack() {
  if (fPosAt) delete fPosAt;
  if (fMomAt) delete fMomAt;
  if (fHelix) delete fHelix;
}

CbmHbtTrack::CbmHbtTrack(const CbmHbtTrack& other) : NicaExpTrack(other) {
  fPosAt = new TVector3(*other.fPosAt);
  fMomAt = new TVector3(*other.fMomAt);
  fHelix = new CbmHelix(*other.fHelix);
  fRPos  = other.fRPos;
  fRMom  = other.fRMom;
}

CbmHbtTrack& CbmHbtTrack::operator=(const CbmHbtTrack& other) {
  if (this == &other) return *this;
  NicaExpTrack::operator=(other);
  *fPosAt               = *other.fPosAt;
  *fMomAt               = *other.fMomAt;
  *fHelix               = *other.fHelix;
  fRPos                 = other.fRPos;
  fRMom                 = other.fRMom;
  return *this;
}

void CbmHbtTrack::CopyData(NicaTrack* other) {
  NicaExpTrack::CopyData(other);
  *fHelix = *((NicaCbmATTrack*) other)->GetHelix();
  fRPos = fRMom = -1;
}

void CbmHbtTrack::CalculatePosAt(Double_t R) {
  if (fRPos == R) return;
  TLorentzVector* vertex = GetEvent()->GetVertex();
  TVector3 pos           = GetHelix()->Eval(vertex->Z() + R);
  fPosAt->SetXYZ(
    pos.X() - vertex->X(), pos.Y() - vertex->Y(), pos.Z() - vertex->Z());
  fRPos = R;
}

void CbmHbtTrack::CalculateMomAt(Double_t R) {
  if (fRMom == R) return;
  TLorentzVector* vertex = GetEvent()->GetVertex();
  GetHelix()->Eval(vertex->Z() + R, *fMomAt);
  fRMom = R;
}
