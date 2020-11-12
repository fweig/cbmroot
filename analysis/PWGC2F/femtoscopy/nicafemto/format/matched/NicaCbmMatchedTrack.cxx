/*
 * NicaCbmMatchedTrack.cxx
 *
 *  Created on: 12 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "NicaCbmMatchedTrack.h"
#include "NicaCbmEvent.h"
#include "NicaCbmMatchedEventInterface.h"

NicaCbmMatchedTrack::NicaCbmMatchedTrack()
  : fStsMCIndex(-1)
  , fTrdMCIndex(-1)
  , fTofMCIndex(-1)
  , fRichMCIndex(-1)
  , fMuchMCIndex(-1) {}

NicaCbmMatchedTrack::~NicaCbmMatchedTrack() {}

void NicaCbmMatchedTrack::Update(CbmGlobalTrack* track,
                                 NicaCbmGlobalEventInterface* interface) {
  NicaCbmTrack::Update(track, interface);
  NicaCbmMatchedEventInterface* ev = (NicaCbmMatchedEventInterface*) interface;
  CbmTrackMatchNew* match_sts      = NULL;
  CbmTrackMatchNew* match_tof      = NULL;
  CbmTrackMatchNew* match_trd      = NULL;
  CbmTrackMatchNew* match_rich     = NULL;
  CbmTrackMatchNew* match_much     = NULL;

  if (GetStsTrackIndex() >= 0)
    match_sts =
      (CbmTrackMatchNew*) ev->GetStsMatches()->UncheckedAt(GetStsTrackIndex());
  if (GetTofHitIndex() >= 0)
    match_tof =
      (CbmTrackMatchNew*) ev->GetTofMatches()->UncheckedAt(GetTofHitIndex());


  if (match_sts) {
    fStsMCIndex = match_sts->GetMatchedLink().GetIndex();
  } else {
    fStsMCIndex = -1;
  }
  if (match_tof) {
    fTofMCIndex = match_tof->GetMatchedLink().GetIndex();
  } else {
    fTofMCIndex = -1;
  }
  NicaCbmSetup mode = ev->GetMode();
  switch (mode) {
    case NicaCbmSetup::kSis100Electron: {
      if (GetTrdTrackIndex() >= 0)
        match_trd = (CbmTrackMatchNew*) ev->GetTrdMatches()->UncheckedAt(
          GetTrdTrackIndex());
      if (GetRichRingIndex() >= 0)
        match_rich = (CbmTrackMatchNew*) ev->GetRichMatches()->UncheckedAt(
          GetRichRingIndex());
      if (match_rich) {
        fRichMCIndex = match_rich->GetMatchedLink().GetIndex();
      } else {
        fRichMCIndex = -1;
      }
      if (match_trd) {
        fTrdMCIndex = match_trd->GetMatchedLink().GetIndex();
      } else {
        fTrdMCIndex = -1;
      }
    } break;
    case NicaCbmSetup::kSis100Hadron: {
      if (GetTrdTrackIndex() >= 0)
        match_trd = (CbmTrackMatchNew*) ev->GetTrdMatches()->UncheckedAt(
          GetTrdTrackIndex());
      if (match_trd) {
        fTrdMCIndex = match_trd->GetMatchedLink().GetIndex();
      } else {
        fTrdMCIndex = -1;
      }
    } break;
    case NicaCbmSetup::kSis100Muon: {
      if (GetTrdTrackIndex() >= 0)
        match_trd = (CbmTrackMatchNew*) ev->GetTrdMatches()->UncheckedAt(
          GetTrdTrackIndex());
      if (GetMuchIndex() >= 0)
        match_much =
          (CbmTrackMatchNew*) ev->GetMuchMatches()->UncheckedAt(GetMuchIndex());
      if (match_much) {
        fMuchMCIndex = match_much->GetMatchedLink().GetIndex();
      } else {
        fMuchMCIndex = -1;
      }
      if (match_trd) {
        fTrdMCIndex = match_trd->GetMatchedLink().GetIndex();
      } else {
        fTrdMCIndex = -1;
      }
    } break;
    default: break;
  }
}

void NicaCbmMatchedTrack::CopyData(NicaTrack* track) {
  NicaCbmTrack::CopyData(track);
  NicaCbmMatchedTrack* tr = (NicaCbmMatchedTrack*) track;
  fStsMCIndex             = tr->GetStsMcIndex();
  fTofMCIndex             = tr->GetTofMcIndex();
  fTrdMCIndex             = tr->GetTrdMcIndex();
  fRichMCIndex            = tr->GetRichMcIndex();
  fMuchMCIndex            = tr->GetMuchMcIndex();
}
