/*
 * NicaEventModuloCut.h
 *
 *  Created on: 8 lut 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAEVENTMODULOCUT_H_
#define NICAEVENTMODULOCUT_H_
#include "NicaEventCut.h"

/**
 * class for rejecting /accpeting events with fixed interwal
 * accept classes only with modulo given by cut min max
 * important note - this class use own counter not event number !
 */
class NicaEventModuloCut : public NicaEventCut {
  Int_t fCount;
  Int_t fModulo;

 public:
  NicaEventModuloCut(Int_t modulo = 1);
  void SetModulo(Int_t modulo) { fModulo = modulo; };
  Bool_t Pass(NicaEvent* event);
  virtual ~NicaEventModuloCut();
  virtual NicaPackage* Report() const;
  ClassDef(NicaEventModuloCut, 1)
};

#endif /* NICAEVENTMODULOCUT_H_ */
