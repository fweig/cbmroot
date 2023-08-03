/*
 * NicaFlowYVariable.h
 *
 *  Created on: 05-10-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFLOWYVARIABLE_H_
#define NICAFLOWYVARIABLE_H_

#include "NicaFlowVariable.h"

/**
 * class used for drawing plot vs rapidity
 */
class NicaFlowYVariable : public NicaFlowVariable {
 public:
  NicaFlowYVariable();
  virtual Double_t GetVariable(NicaTrack* p);
  virtual NicaFlowVariable* MakeCopy() const {
    return new NicaFlowYVariable(*this);
  };
  virtual ~NicaFlowYVariable();
  ClassDef(NicaFlowYVariable, 1)
};

#endif /* NICAFLOWYVARIABLE_H_ */
