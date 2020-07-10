/*
 * NicaCbmEvent.h
 *
 *  Created on: 3 gru 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACBMGLOBEVENT_H_
#define NICACBMGLOBEVENT_H_

#include "NicaExpEvent.h"

class NicaCbmGlobalEvent : public NicaExpEvent {
protected:
  virtual void ShallowCopyTracks(NicaEvent* event);
  virtual void ShallowCopyEvent(NicaEvent* event);
  virtual void CreateSource();
  NicaCbmGlobalEvent(TString classname);

public:
  NicaCbmGlobalEvent();
  NicaCbmGlobalEvent(const NicaCbmGlobalEvent& other);
  virtual void Update();
  virtual Bool_t ExistInTree() const;
  virtual TString GetFormatName() const { return "CbmGlobalEvent"; };
  virtual ~NicaCbmGlobalEvent();
  ClassDef(NicaCbmGlobalEvent, 1)
};

#endif /* NICACBMGLOBEVENT_H_ */
