/*
 * NicaCbmMatchedEvent.cxx
 *
 *  Created on: 12 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "NicaCbmMatchedEvent.h"
#include "FairRootManager.h"
#include "NicaCbmMatchedEventInterface.h"
#include "NicaCbmMatchedTrack.h"
void NicaCbmMatchedEvent::CreateSource() {
  fSource = new NicaCbmMatchedEventInterface(GetMode());
}

NicaCbmMatchedEvent::NicaCbmMatchedEvent(NicaCbmSetup mode)
  : NicaCbmEvent("NicaCbmMatchedTrack", mode) {}

Bool_t NicaCbmMatchedEvent::ExistInTree() const {
  switch (fMode) {
    case NicaCbmSetup::kSis100Electron:
      if (!CheckBranches(4,
                         "StsTrackMatch",
                         "TofHitMatch",
                         "TrdTrackMatch",
                         "RichRingMatch"))
        return kFALSE;
      break;
    case NicaCbmSetup::kSis100Muon:
      if (!CheckBranches(4,
                         "StsTrackMatch",
                         "TofHitMatch",
                         "TrdTrackMatch",
                         "MuchTrackMatch"))
        return kFALSE;
      break;
    case NicaCbmSetup::kSis100Hadron: {
      if (!CheckBranches(3, "StsTrackMatch", "TofHitMatch", "TrdTrackMatch"))
        return kFALSE;
    } break;
    default:
      if (!CheckBranches(2, "StsTrackMatch", "TofHitMatch")) return kFALSE;
      break;
  }
  return NicaCbmEvent::ExistInTree();
}

NicaCbmMatchedEvent::~NicaCbmMatchedEvent() {}

NicaCbmMatchedEvent::NicaCbmMatchedEvent(TString classname, NicaCbmSetup mode)
  : NicaCbmEvent(classname, mode) {}
