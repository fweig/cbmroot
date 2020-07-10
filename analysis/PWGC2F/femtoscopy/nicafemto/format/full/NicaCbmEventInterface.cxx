/*
 * NicaCbmMatchedEventInterface.cxx
 *
 *  Created on: 11 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "NicaCbmEventInterface.h"

#include "CbmGlobalTrack.h"
#include "CbmLink.h"
#include "CbmMuchTrack.h"
#include "CbmRichRing.h"
#include "CbmStsTrack.h"
#include "CbmTofHit.h"
#include "CbmTrdTrack.h"
#include "NicaCbmEvent.h"
#include "NicaCbmTrack.h"
#include "NicaCbmTrackInterface.h"
#include "NicaTrackClones.h"
#include <FairRootManager.h>
#include <RtypesCore.h>
#include <TClonesArray.h>

NicaCbmEventInterface::NicaCbmEventInterface(NicaCbmSetup mode)
  : fMode(mode), fMuchTracks(NULL), fRichRings(NULL) {
  fStsTracks = new NicaTrackClones("CbmStsTrack", "StsTrack", "STS");
  fTofHits   = new NicaTrackClones("CbmTofHit", "TofHit", "TOF");
  fTrdTracks = new NicaTrackClones("CbmTrdTrack", "TrdTrack", "TRD");
  switch (fMode) {
    case kSis100Electron: {
      fRichRings = new NicaTrackClones("CbmRichRing", "RichRing", "");
    } break;
    case kSis100Hadron: {

    } break;
    case kSis100Muon: {
      fMuchTracks = new NicaTrackClones("CbmMuchTrack", "MuchTrack", "MUCH");
    } break;
  }
}

void NicaCbmEventInterface::ConnectToTree() {
  NicaCbmGlobalEventInterface::ConnectToTree();
  fStsTracks->GetFromTree();
  fTofHits->GetFromTree();
  fTrdTracks->GetFromTree();
  switch (fMode) {
    case kSis100Electron: {
      fRichRings->GetFromTree();
    } break;
    case kSis100Hadron: {

    } break;
    case kSis100Muon: {
      fMuchTracks->GetFromTree();
    } break;
  }
}

void NicaCbmEventInterface::Register(Bool_t write) {
  NicaCbmGlobalEventInterface::Register(write);
  fStsTracks->Register(write);
  fTofHits->Register(write);
  fTrdTracks->Register(write);
  switch (fMode) {
    case kSis100Electron: {
      fRichRings->Register(write);
    } break;
    case kSis100Hadron: break;
    case kSis100Muon: {
      fMuchTracks->Register(write);
    } break;
  }
}

void NicaCbmEventInterface::CopyData(NicaEventInterface* s) {
  NicaCbmGlobalEventInterface::CopyData(s);
  NicaCbmEventInterface* interface = (NicaCbmEventInterface*) s;
  fStsTracks->CopyFrom<CbmStsTrack>(interface->fStsTracks->GetArray());
  fTofHits->CopyFrom<CbmTofHit>(interface->fTofHits->GetArray());
  fTrdTracks->CopyFrom<CbmTrdTrack>(interface->fTrdTracks->GetArray());
  switch (fMode) {
    case kSis100Electron: {
      fRichRings->CopyFrom<CbmRichRing>(interface->fRichRings->GetArray());
    } break;
    case kSis100Hadron: break;
    case kSis100Muon: {
      fMuchTracks->CopyFrom<CbmMuchTrack>(interface->fMuchTracks->GetArray());
    } break;
  }
}

void NicaCbmEventInterface::Compress(Int_t* map, Int_t map_size) {
  NicaCbmGlobalEventInterface::Compress(map, map_size);
  //TODO make compression
}

void NicaCbmEventInterface::CopyAndCompress(NicaEventInterface* s,
                                            Int_t* map,
                                            Int_t map_size) {
  NicaCbmGlobalEventInterface::CopyAndCompress(s, map, map_size);
  NicaCbmEventInterface* interface = (NicaCbmEventInterface*) s;
  //TODO compression
  fStsTracks->CopyFrom<CbmStsTrack>(interface->fStsTracks->GetArray());
  fTofHits->CopyFrom<CbmTofHit>(interface->fTofHits->GetArray());
  fTrdTracks->CopyFrom<CbmTrdTrack>(interface->fTrdTracks->GetArray());
  if (fRichRings)
    fRichRings->CopyFrom<CbmRichRing>(interface->fRichRings->GetArray());
  if (fMuchTracks)
    fMuchTracks->CopyFrom<CbmMuchTrack>(interface->fMuchTracks->GetArray());
}

void NicaCbmEventInterface::FillTrackInterface(NicaTrackInterface* track,
                                               Int_t index) {
  NicaCbmTrackInterface* cbm_track = (NicaCbmTrackInterface*) track;
  cbm_track->SetRawTrack(fTracks->UncheckedAt(index));
  CbmGlobalTrack* glob = (CbmGlobalTrack*) fTracks->UncheckedAt(index);
  cbm_track->SetStsTrack(
    (CbmStsTrack*) fStsTracks->UncheckedAt(glob->GetStsTrackIndex()));
  cbm_track->SetTofHit(
    (CbmTofHit*) fTofHits->UncheckedAt(glob->GetTofHitIndex()));
  cbm_track->SetTrdTrack(
    (CbmTrdTrack*) fTrdTracks->UncheckedAt(glob->GetTrdTrackIndex()));
  switch (fMode) {
    case kSis100Electron: {
      cbm_track->SetRichRing(
        (CbmRichRing*) fRichRings->UncheckedAt(glob->GetRichRingIndex()));
    } break;
    case kSis100Hadron: break;
    case kSis100Muon: {
      cbm_track->SetMuchTrack(
        (CbmMuchTrack*) fMuchTracks->UncheckedAt(glob->GetMuchTrackIndex()));
    } break;
  }
}

TObject* NicaCbmEventInterface::GetRawTrackPointer(Int_t index) const {
  return NicaCbmGlobalEventInterface::GetRawTrackPointer(index);
}

NicaCbmEventInterface::~NicaCbmEventInterface() {
  if (CanDeleteEvent()) {
    if (fStsTracks) delete fStsTracks;
    if (fTofHits) delete fTofHits;
    if (fTrdTracks) delete fTrdTracks;
    if (fRichRings) delete fRichRings;
    if (fMuchTracks) delete fMuchTracks;
  }
}
