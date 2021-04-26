/*
 * NicaCbmTrack.cxx
 *
 *  Created on: 3 gru 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "NicaCbmTrack.h"

#include "CbmHelix.h"

#include "NicaCbmEventInterface.h"
#include "NicaDataFormat.h"
#include "NicaExpEvent.h"

NicaCbmTrack::NicaCbmTrack() : fTofTrack(new NicaToFTrack()), fHelix(new CbmHelix()), fChi2Vertex(0), fMvdHits(0) {}

NicaCbmTrack::NicaCbmTrack(const NicaCbmTrack& other)
  : NicaExpTrack(other)
  , fTofTrack(new NicaToFTrack(*other.fTofTrack))
  , fHelix(new CbmHelix(*other.fHelix))
  , fChi2Vertex(other.fChi2Vertex)
  , fMvdHits(other.fMvdHits)
{
}

NicaCbmTrack& NicaCbmTrack::operator=(const NicaCbmTrack& other)
{
  if (this == &other) return *this;
  NicaExpTrack::operator=(other);
  *fTofTrack            = *other.fTofTrack;
  *fHelix               = *other.fHelix;
  fChi2Vertex           = other.fChi2Vertex;
  fMvdHits              = other.fMvdHits;
  return *this;
}

TObject* NicaCbmTrack::GetDetTrack(const UInt_t detID) const
{
  if (detID == NicaDetectorID::kTOF) { return fTofTrack; }
  return nullptr;
}

NicaCbmTrack::~NicaCbmTrack()
{
  delete fTofTrack;
  delete fHelix;
}

void NicaCbmTrack::CopyData(NicaTrack* other)
{
  NicaExpTrack::CopyData(other);
  *fTofTrack  = *(static_cast<NicaCbmTrack*>(other))->fTofTrack;
  *fHelix     = *(static_cast<NicaCbmTrack*>(other))->fHelix;
  fChi2Vertex = (static_cast<NicaCbmTrack*>(other))->fChi2Vertex;
  fMvdHits    = (static_cast<NicaCbmTrack*>(other))->fMvdHits;
}
