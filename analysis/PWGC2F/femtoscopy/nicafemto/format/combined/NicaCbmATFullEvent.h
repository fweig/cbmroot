/*
 * NicaCbmATEvent.h
 *
 *  Created on: 17 wrz 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACBMATEVENT_H_
#define NICACBMATEVENT_H_
#include "NicaComplexEvent.h"

/**
 * class for analyses that base on anatree events
 */

class NicaCbmATFullEvent : public NicaComplexEvent {
public:
  NicaCbmATFullEvent();
  virtual void Update();
  virtual TString GetFormatName() const { return "NicaGlobalMCEvent"; };
  virtual NicaEvent* GetNewEvent() const;
  virtual ~NicaCbmATFullEvent();
  ClassDef(NicaCbmATFullEvent, 1)
};

#endif /* NICACBMATEVENT_H_ */
