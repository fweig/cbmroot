/*
 * NicaCbmDstMcEvent.h
 *
 *  Created on: 17 wrz 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACBMATMCEVENT_H_
#define NICACBMATMCEVENT_H_

#include "NicaMCEvent.h"

class NicaCbmATMCEvent : public NicaMCEvent {
protected:
  virtual void CreateSource();

public:
  NicaCbmATMCEvent();
  NicaCbmATMCEvent(const NicaCbmATMCEvent& other);
  virtual void Update();
  virtual NicaEvent* GetNewEvent() const { return new NicaCbmATMCEvent(); };
  virtual Bool_t ExistInTree() const;
  virtual TString GetFormatName() const { return "CbmATMCEvent"; };
  virtual ~NicaCbmATMCEvent();
  ClassDef(NicaCbmATMCEvent, 1)
};

#endif /* NICACBMATMCEVENT_H_ */
