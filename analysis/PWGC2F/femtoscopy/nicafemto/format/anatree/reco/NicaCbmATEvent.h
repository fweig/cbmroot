/*
 * NicaCbmEvent.h
 *
 *  Created on: 3 gru 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACBMDSTEVENT_H_
#define NICACBMDSTEVENT_H_

#include "NicaCbmATTrack.h"
#include "NicaExpEvent.h"

class NicaCbmATEvent : public NicaExpEvent {
protected:
  virtual void ShallowCopyTracks(NicaEvent* event);
  virtual void ShallowCopyEvent(NicaEvent* event);
  virtual void CreateSource();
  NicaCbmATEvent(TString classname);

public:
  NicaCbmATEvent();
  NicaCbmATEvent(const NicaCbmATEvent& other);
  virtual void Update();
  virtual Bool_t ExistInTree() const;
  NicaTrack* GetNewTrack() const { return new NicaCbmATTrack(); };
  virtual NicaEvent* GetNewEvent() const { return new NicaCbmATEvent(); };
  virtual TString GetFormatName() const { return "NicaCbmATEvent"; };
  virtual ~NicaCbmATEvent();
  ClassDef(NicaCbmATEvent, 1)
};

#endif /* NICACBMDSTEVENT_H_ */
