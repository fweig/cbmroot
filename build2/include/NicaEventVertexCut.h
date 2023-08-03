/*
 * NicaEventVertexCut.h
 *
 *  Created on: 4 cze 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAEVENTVERTEXCUT_H_
#define NICAEVENTVERTEXCUT_H_
#include "NicaEventCut.h"

class NicaEventVertexCut : public NicaEventCut {
 public:
  NicaEventVertexCut();
  static Int_t Rt() { return 0; }
  static Int_t Z() { return 1; };
  virtual Bool_t Init(Int_t format_id);
  virtual Bool_t Pass(NicaEvent *event);
  virtual ~NicaEventVertexCut();
  ClassDef(NicaEventVertexCut, 1)
};

#endif /* NICAEVENTVERTEXCUT_H_ */
