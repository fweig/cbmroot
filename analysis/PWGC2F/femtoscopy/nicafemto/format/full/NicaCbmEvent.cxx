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
  , fTrdTracks(NULL)
  , fRichRings(NULL)
  , fMuchTracks(NULL) {
  fStsTracks = new NicaTrackClones("CbmStsTrack", "", "");
  fTofHits   = new NicaTrackClones("CbmTofHit", "", "");

  switch (fMode) {
    case NicaCbmSetup::kSis100Electron:
      fRichRings = new NicaTrackClones("CbmRichRing", "", "");
      fTrdTracks = new NicaTrackClones("CbmTrdTrack", "", "");
      break;
    case NicaCbmSetup::kSis100Hadron:
      fTrdTracks = new NicaTrackClones("CbmTrdTrack", "", "");
      break;
    case NicaCbmSetup::kSis100Muon:
      fMuchTracks = new NicaTrackClones("CbmMuchTrack", "", "");
      fTrdTracks  = new NicaTrackClones("CbmTrdTrack", "", "");
      break;
    case NicaCbmSetup::kSis100Mini: break;
  }
}

void NicaCbmEvent::ShallowCopyTracks(NicaEvent* event) {
  NicaCbmEvent* ev = (NicaCbmEvent*) event;
  fStsTracks->CopyFrom<CbmStsTrack>(ev->fStsTracks->GetArray());
  fTofHits->CopyFrom<CbmTofHit>(ev->fTofHits->GetArray());
  switch (fMode) {
    case NicaCbmSetup::kSis100Electron:
      fTrdTracks->CopyFrom<CbmTrdTrack>(ev->fTrdTracks->GetArray());
      fRichRings->CopyFrom<CbmRichRing>(ev->fRichRings->GetArray());
      break;
    case NicaCbmSetup::kSis100Hadron:
      fTrdTracks->CopyFrom<CbmTrdTrack>(ev->fTrdTracks->GetArray());
      break;
    case NicaCbmSetup::kSis100Muon:
      fMuchTracks->CopyFrom<CbmMuchTrack>(ev->fMuchTracks->GetArray());
      fTrdTracks->CopyFrom<CbmTrdTrack>(ev->fTrdTracks->GetArray());
      break;
    case NicaCbmSetup::kSis100Mini: break;
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
  switch (fMode) {
    case NicaCbmSetup::kSis100Electron:
      fTrdTracks->CopyFrom<CbmTrdTrack>(interface->fTrdTracks->GetArray());
      fRichRings->CopyFrom<CbmRichRing>(interface->fRichRings->GetArray());
      break;
    case NicaCbmSetup::kSis100Hadron:
      fTrdTracks->CopyFrom<CbmTrdTrack>(interface->fTrdTracks->GetArray());
      break;
    case NicaCbmSetup::kSis100Muon:
      fTrdTracks->CopyFrom<CbmTrdTrack>(interface->fTrdTracks->GetArray());
      fMuchTracks->CopyFrom<CbmMuchTrack>(interface->fMuchTracks->GetArray());
      break;
    case NicaCbmSetup::kSis100Mini: break;
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
  switch (fMode) {
    case NicaCbmSetup::kSis100Electron: {
      fTrdTracks = new NicaTrackClones("CbmTrdTrack", "TrdTrack", "TRD");
      fRichRings = new NicaTrackClones("CbmRichRing", "RichRing", "");
    } break;
    case NicaCbmSetup::kSis100Hadron:
      fTrdTracks = new NicaTrackClones("CbmTrdTrack", "TrdTrack", "TRD");
      break;
    case NicaCbmSetup::kSis100Muon: {
      fTrdTracks  = new NicaTrackClones("CbmTrdTrack", "TrdTrack", "TRD");
      fMuchTracks = new NicaTrackClones("CbmMuchTrack", "MuchTrack", "MUCH");
    } break;
    case NicaCbmSetup::kSis100Mini: {

    } break;
  }
}

Bool_t NicaCbmEvent::ExistInTree() const {
  Bool_t exists = kFALSE;
  switch (fMode) {
    case NicaCbmSetup::kSis100Electron:
      exists = CheckBranches(4, "StsTrack", "TofHit", "TrdTrack", "RichRing");
      break;
    case NicaCbmSetup::kSis100Muon:
      exists = CheckBranches(4, "StsTrack", "TofHit", "TrdTrack", "MuchTrack");
      break;
    case NicaCbmSetup::kSis100Hadron:
      exists = CheckBranches(3, "StsTrack", "TofHit", "TrdTrack");
      break;
    case NicaCbmSetup::kSis100Mini:
      exists = CheckBranches(2, "StsTrack", "TofHit");
      break;
  }
  if (exists == kFALSE) return kFALSE;
  return NicaCbmGlobalEvent::ExistInTree();
}

NicaCbmEvent::~NicaCbmEvent() {
  delete fStsTracks;
  delete fTofHits;
  if (fTrdTracks) delete fTrdTracks;
  if (fRichRings) delete fRichRings;
  if (fMuchTracks) delete fMuchTracks;
}
