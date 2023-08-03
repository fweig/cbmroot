/*
 * NicaEventPrimaryNoCut.h
 *
 *  Created on: 15 mar 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAEVENTPRIMARYTRACKSNOCUT_H_
#define NICAEVENTPRIMARYTRACKSNOCUT_H_

#include "NicaEventCut.h"

class NicaEventPrimaryTracksNoCut : public NicaEventCut {
 public:
  NicaEventPrimaryTracksNoCut();
  virtual Bool_t Pass(NicaEvent *event);
  virtual ~NicaEventPrimaryTracksNoCut();
  ClassDef(NicaEventPrimaryTracksNoCut, 1)
};

#endif /* NICAEVENTPRIMARYTRACKSNOCUT_H_ */
