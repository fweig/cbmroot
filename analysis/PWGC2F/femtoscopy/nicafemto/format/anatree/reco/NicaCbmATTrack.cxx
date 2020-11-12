/*
 * NicaCbmTrack.cxx
 *
 *  Created on: 3 gru 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "NicaCbmATTrack.h"

#include "CbmHelix.h"
#include "NicaCbmATEventInterface.h"
#include "NicaDetectorID.h"
#include "NicaExpEvent.h"

NicaCbmATTrack::NicaCbmATTrack()
  : fTofTrack(new NicaToFTrack()), fHelix(new CbmHelix()), fChi2Vertex(0) {}

NicaCbmATTrack::NicaCbmATTrack(const NicaCbmATTrack& other)
  : NicaExpTrack(other)
  , fTofTrack(new NicaToFTrack(*other.fTofTrack))
  , fHelix(new CbmHelix(*other.fHelix))
  , fChi2Vertex(other.fChi2Vertex) {}

NicaCbmATTrack& NicaCbmATTrack::operator=(const NicaCbmATTrack& other) {
  if (this == &other) return *this;
  NicaExpTrack::operator=(other);
  *fTofTrack            = *other.fTofTrack;
  *fHelix               = *other.fHelix;
  fChi2Vertex           = other.fChi2Vertex;
  return *this;
}

TObject* NicaCbmATTrack::GetDetTrack(const UInt_t detID) const {
  if (detID == NicaDetectorID::kTOF) { return fTofTrack; }
  return nullptr;
}

NicaCbmATTrack::~NicaCbmATTrack() {
  delete fTofTrack;
  delete fHelix;
}

void NicaCbmATTrack::CopyData(NicaTrack* other) {
  NicaExpTrack::CopyData(other);
  *fTofTrack  = *(static_cast<NicaCbmATTrack*>(other))->fTofTrack;
  *fHelix     = *(static_cast<NicaCbmATTrack*>(other))->fHelix;
  fChi2Vertex = (static_cast<NicaCbmATTrack*>(other))->fChi2Vertex;
}
