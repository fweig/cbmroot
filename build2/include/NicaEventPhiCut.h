/*
 * NicaEventPhiCut.h
 *
 *  Created on: 18-03-2014
 *      Author: wielanek
 */

#ifndef NICAEVENTPHICUT_H_
#define NICAEVENTPHICUT_H_

#include "NicaEventCut.h"
/**
 * check reaction plane angle
 */
class NicaEventPhiCut : public NicaEventCut {
 protected:
 public:
  NicaEventPhiCut();
  virtual Bool_t Pass(NicaEvent *event);
  virtual ~NicaEventPhiCut();
  ClassDef(NicaEventPhiCut, 1)
};

#endif /* NICAEVENTPHICUT_H_ */
