/*
 * NicaEventPtMaxCut.h
 *
 *  Created on: 11-11-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICAEVENTPTMAXCUT_H_
#define NICAEVENTPTMAXCUT_H_

#include "NicaEventCut.h"
/**
 * check biggest track pt in event
 */
class NicaEventPtMaxCut : public NicaEventCut {
 public:
  NicaEventPtMaxCut();
  virtual Bool_t Pass(NicaEvent *event);
  virtual Bool_t Init(Int_t task_id = 0);
  virtual ~NicaEventPtMaxCut();
  ClassDef(NicaEventPtMaxCut, 1)
};

#endif /* NICAEVENTPTMAXCUT_H_ */
