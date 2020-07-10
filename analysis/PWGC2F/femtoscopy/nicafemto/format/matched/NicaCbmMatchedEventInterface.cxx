/*
 * NicaCbmMatchedEventInterface.cxx
 *
 *  Created on: 12 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "NicaCbmMatchedEventInterface.h"
#include "NicaCbmEvent.h"
#include "NicaCbmMatchedTrackInterface.h"

NicaCbmMatchedEventInterface::NicaCbmMatchedEventInterface(NicaCbmSetup mode)
  : NicaCbmEventInterface(mode), fMuchMatches(NULL), fRichMatches(NULL) {
  fStsMatches = new NicaTrackClones("CbmTrackMatchNew", "StsTrackMatch", "STS");
  fTofMatches = new NicaTrackClones("CbmTrackMatchNew", "TofHitMatch", "TOF");
  fTrdMatches = new NicaTrackClones("CbmTrackMatchNew", "TrdTrackMatch", "TRD");
  switch (fMode) {
    case kSis100Electron: {
      fRichMatches =
        new NicaTrackClones("CbmTrackMatchNew", "RichRingMatch", "RICH");
    } break;
    case kSis100Hadron: break;
    case kSis100Muon: {
      fMuchMatches =
        new NicaTrackClones("CbmTrackMatchNew", "MuchTrackMatch", "MUCH");
    } break;
  }
}

void NicaCbmMatchedEventInterface::ConnectToTree() {
  NicaCbmEventInterface::ConnectToTree();
  if (CanDeleteEvent()) {
    fStsMatches->DeleteClones();
    fTofMatches->DeleteClones();
    fTrdMatches->DeleteClones();
    switch (fMode) {
      case kSis100Electron: {
        fRichMatches->DeleteClones();
      } break;
      case kSis100Hadron: break;
      case kSis100Muon: {
        fMuchMatches->DeleteClones();
      } break;
    }
  }
  fStsMatches->GetFromTree();
  fTofMatches->GetFromTree();
  fTrdMatches->GetFromTree();
  if (fRichMatches) fRichMatches->GetFromTree();
  if (fMuchMatches) fMuchMatches->GetFromTree();
}

void NicaCbmMatchedEventInterface::Register(Bool_t write) {
  NicaCbmEventInterface::Register(write);
  fStsMatches->Register(write);
  fTofMatches->Register(write);
  fTrdMatches->Register(write);
  if (fMuchMatches) fMuchMatches->Register(write);
  if (fRichMatches) fRichMatches->Register(write);
}

void NicaCbmMatchedEventInterface::CopyData(NicaEventInterface* s) {
  NicaCbmEventInterface::CopyData(s);
  NicaCbmMatchedEventInterface* interface = (NicaCbmMatchedEventInterface*) s;
  fStsMatches->CopyFrom<CbmTrackMatchNew>(interface->GetStsMatches());
  fTofMatches->CopyFrom<CbmTrackMatchNew>(interface->GetTofMatches());
  fTrdMatches->CopyFrom<CbmTrackMatchNew>(interface->GetTrdMatches());
  if (fMuchMatches)
    fMuchMatches->CopyFrom<CbmTrackMatchNew>(interface->GetMuchMatches());
  if (fRichMatches)
    fRichMatches->CopyFrom<CbmTrackMatchNew>(interface->GetRichMatches());
}

void NicaCbmMatchedEventInterface::Compress(Int_t* map, Int_t map_size) {
  NicaCbmEventInterface::Compress(map, map_size);
  //TODO make compression
}

void NicaCbmMatchedEventInterface::CopyAndCompress(NicaEventInterface* s,
                                                   Int_t* map,
                                                   Int_t map_size) {
  NicaCbmEventInterface::CopyAndCompress(s, map, map_size);
  NicaCbmMatchedEventInterface* interface = (NicaCbmMatchedEventInterface*) s;
  fStsMatches->CopyFrom<CbmTrackMatchNew>(interface->GetStsMatches());
  fTofMatches->CopyFrom<CbmTrackMatchNew>(interface->GetTofMatches());
  fTrdMatches->CopyFrom<CbmTrackMatchNew>(interface->GetTrdMatches());
  if (fMuchMatches)
    fMuchMatches->CopyFrom<CbmTrackMatchNew>(interface->GetMuchMatches());
  if (fRichMatches)
    fRichMatches->CopyFrom<CbmTrackMatchNew>(interface->GetRichMatches());
}

void NicaCbmMatchedEventInterface::FillTrackInterface(NicaTrackInterface* track,
                                                      Int_t index) {
  NicaCbmEventInterface::FillTrackInterface(track, index);
  NicaCbmMatchedTrackInterface* Track = (NicaCbmMatchedTrackInterface*) track;
  CbmGlobalTrack* glob = (CbmGlobalTrack*) fTracks->UncheckedAt(index);
  Track->SetLinkSts(
    (CbmTrackMatchNew*) fStsMatches->UncheckedAt(glob->GetStsTrackIndex()));
  Track->SetLinkTof(
    (CbmTrackMatchNew*) fTofMatches->UncheckedAt(glob->GetTofHitIndex()));
  Track->SetLinkTrd(
    (CbmTrackMatchNew*) fTrdMatches->UncheckedAt(glob->GetTrdTrackIndex()));
  if (fMuchMatches) {
    Track->SetLinkMuch(
      (CbmTrackMatchNew*) fMuchMatches->UncheckedAt(glob->GetMuchTrackIndex()));
  }
  if (fRichMatches) {
    Track->SetLinkRich(
      (CbmTrackMatchNew*) fRichMatches->UncheckedAt(glob->GetRichRingIndex()));
  }
}

NicaCbmMatchedEventInterface::~NicaCbmMatchedEventInterface() {
  if (CanDeleteEvent()) {
    if (fStsMatches) delete fStsMatches;
    if (fTofMatches) delete fTofMatches;
    if (fTrdMatches) delete fTrdMatches;
    if (fRichMatches) delete fRichMatches;
    if (fMuchMatches) delete fMuchMatches;
  }
}
