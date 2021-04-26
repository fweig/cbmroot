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

class NicaCbmMCEvent : public NicaMCEvent {
protected:
  virtual void CreateSource();

public:
  NicaCbmMCEvent();
  NicaCbmMCEvent(const NicaCbmMCEvent& other);
  virtual void Update();
  virtual NicaEvent* GetNewEvent() const { return new NicaCbmMCEvent(); };
  virtual Bool_t ExistInTree() const;
  virtual TString GetFormatName() const { return "CbmMCEvent"; };
  virtual ~NicaCbmMCEvent();
  ClassDef(NicaCbmMCEvent, 1)
};

#endif /* NICACBMATMCEVENT_H_ */
