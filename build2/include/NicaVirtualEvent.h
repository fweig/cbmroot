/*
 * NicaEventVirtual.h
 *
 *  Created on: 4 kwi 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAVIRTUALEVENT_H_
#define NICAVIRTUALEVENT_H_

#include <TClonesArray.h>
#include <TString.h>

#include "NicaEvent.h"
#include "NicaVirtualTrack.h"

class NicaVirtualEvent : public NicaEvent {
  NicaEvent* fVirtualEvent;

 protected:
  void DeleteSource(){};
  void LinkTrackPointers(){};
  void CopySource(NicaEvent* /*event*/){};
  void CopyAndCompressSource(NicaEvent* /*event*/, Int_t* /*map*/,
                             Int_t /*map_size*/){};
  void CompressSource(Int_t* /*map*/, Int_t /*map_size*/){};

 public:
  NicaVirtualEvent();
  NicaVirtualEvent(const NicaVirtualEvent& other);
  void CreateSource(){};
  void Update();
  void LinkWithTree();
  void RegisterInTree(TString prefix, Bool_t save = kTRUE);
  Bool_t ExistInTree() const;
  NicaTrack* GetTrackSafely(Int_t i) {
    return (NicaTrack*)fTracks->ConstructedAt(i);
  };
  NicaEvent* GetNewEvent() const { return new NicaVirtualEvent(*this); };
  NicaTrack* GetNewTrack() const { return new NicaVirtualTrack(); };
  virtual ~NicaVirtualEvent();
  ClassDef(NicaVirtualEvent, 1)
};

#endif /* NICAVIRTUALEVENT_H_ */
