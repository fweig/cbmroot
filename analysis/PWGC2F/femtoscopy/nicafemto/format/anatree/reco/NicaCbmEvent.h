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

#include "NicaCbmTrack.h"
#include "NicaExpEvent.h"

class NicaCbmEvent : public NicaExpEvent {
protected:
  virtual void CreateSource();
  NicaCbmEvent(TString classname);

public:
  NicaCbmEvent();
  NicaCbmEvent(const NicaCbmEvent& other);
  virtual void Update();
  virtual Bool_t ExistInTree() const;
  virtual NicaTrack* GetNewTrack() const { return new NicaCbmTrack(); };
  virtual NicaEvent* GetNewEvent() const { return new NicaCbmEvent(); };
  virtual TString GetFormatName() const { return "NicaCbmEvent"; };
  virtual ~NicaCbmEvent();
  ClassDef(NicaCbmEvent, 1)
};

#endif /* NICACBMDSTEVENT_H_ */
