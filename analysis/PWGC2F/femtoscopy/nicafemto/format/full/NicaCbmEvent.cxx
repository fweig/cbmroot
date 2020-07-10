/*
 * NicaCbmMatcheEvent.cxx
 *
 *  Created on: 11 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "NicaCbmEvent.h"
#include "CbmMuchTrack.h"
#include "CbmRichRing.h"
#include "CbmStsTrack.h"
#include "CbmTofHit.h"
#include "CbmTrdTrack.h"
#include "NicaCbmEventInterface.h"
#include "NicaCbmTrack.h"
NicaCbmEvent::NicaCbmEvent(NicaCbmSetup mode)
  : NicaCbmGlobalEvent("NicaCbmTrack")
  , fMode(mode)
  , fRichRings(NULL)
  , fMuchTracks(NULL) {
  fStsTracks = new NicaTrackClones("CbmStsTrack", "", "");
  fTofHits   = new NicaTrackClones("CbmTofHit", "", "");
  fTrdTracks = new NicaTrackClones("CbmTrdTrack", "", "");
  switch (fMode) {
    case kSis100Electron:
      fRichRings = new NicaTrackClones("CbmRichRing", "", "");
      break;
    case kSis100Hadron: break;
    case kSis100Muon:
      fMuchTracks = new NicaTrackClones("CbmMuchTrack", "", "");
      break;
  }
}

void NicaCbmEvent::ShallowCopyTracks(NicaEvent* event) {
  NicaCbmEvent* ev = (NicaCbmEvent*) event;
  fStsTracks->CopyFrom<CbmStsTrack>(ev->fStsTracks->GetArray());
  fTofHits->CopyFrom<CbmTofHit>(ev->fTofHits->GetArray());
  fTrdTracks->CopyFrom<CbmTrdTrack>(ev->fTrdTracks->GetArray());
  switch (fMode) {
    case kSis100Electron:
      fRichRings->CopyFrom<CbmRichRing>(ev->fRichRings->GetArray());
      break;
    case kSis100Hadron: break;
    case kSis100Muon:
      fMuchTracks->CopyFrom<CbmMuchTrack>(ev->fMuchTracks->GetArray());
      break;
  }
  NicaCbmGlobalEvent::ShallowCopyTracks(event);
}

void NicaCbmEvent::ShallowCopyEvent(NicaEvent* event) {
  NicaCbmGlobalEvent::ShallowCopyEvent(event);
}

void NicaCbmEvent::CreateSource() {
  fSource = new NicaCbmEventInterface(fMode);
}

void NicaCbmEvent::Update() {
  NicaCbmEventInterface* interface = (NicaCbmEventInterface*) fSource;
  fStsTracks->CopyFrom<CbmStsTrack>(interface->fStsTracks->GetArray());
  fTofHits->CopyFrom<CbmTofHit>(interface->fTofHits->GetArray());
  fTrdTracks->CopyFrom<CbmTrdTrack>(interface->fTrdTracks->GetArray());
  switch (fMode) {
    case kSis100Electron:
      fRichRings->CopyFrom<CbmRichRing>(interface->fRichRings->GetArray());
      break;
    case kSis100Hadron: break;
    case kSis100Muon:
      fMuchTracks->CopyFrom<CbmMuchTrack>(interface->fMuchTracks->GetArray());
      break;
  }
  NicaCbmGlobalEvent::Update();
}

TString NicaCbmEvent::GetFormatName() const { return "CbmEvent"; }

NicaCbmEvent::NicaCbmEvent(TString classname, NicaCbmSetup mode)
  : NicaCbmGlobalEvent(classname)
  , fMode(mode)
  , fRichRings(NULL)
  , fMuchTracks(NULL) {
  fStsTracks = new NicaTrackClones("CbmStsTrack", "StsTrack", "STS");
  fTofHits   = new NicaTrackClones("CbmTofHit", "TofHit", "TOF");
  fTrdTracks = new NicaTrackClones("CbmTrdTrack", "TrdTrack", "TRD");
  switch (fMode) {
    case kSis100Electron: {
      fRichRings = new NicaTrackClones("CbmRichRing", "RichRing", "");
    } break;
    case kSis100Hadron: break;
    case kSis100Muon: {
      fMuchTracks = new NicaTrackClones("CbmMuchTrack", "MuchTrack", "MUCH");
    } break;
  }
}

Bool_t NicaCbmEvent::ExistInTree() const {
  Bool_t exists = kFALSE;
  switch (fMode) {
    case kSis100Electron:
      exists = CheckBranches(4, "StsTrack", "TofHit", "TrdTrack", "RichRing");
      break;
    case kSis100Muon:
      exists = CheckBranches(4, "StsTrack", "TofHit", "TrdTrack", "MuchTrack");
      break;
    case kSis100Hadron:
      exists = CheckBranches(3, "StsTrack", "TofHit", "TrdTrack");
      break;
  }
  if (exists == kFALSE) return kFALSE;
  return NicaCbmGlobalEvent::ExistInTree();
}

NicaCbmEvent::~NicaCbmEvent() {
  delete fStsTracks;
  delete fTofHits;
  delete fTrdTracks;
  if (fRichRings) delete fRichRings;
  if (fMuchTracks) delete fMuchTracks;
}
