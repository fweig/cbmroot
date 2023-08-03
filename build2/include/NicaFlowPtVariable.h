/*
 * NicaFlowPtVariable.h
 *
 *  Created on: 19-10-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICAFLOWPTVARIABLE_H_
#define NICAFLOWPTVARIABLE_H_

#include "NicaFlowVariable.h"
/**
 * class that is used for drawing flow vs transverse momentum
 */
class NicaFlowPtVariable : public NicaFlowVariable {
 public:
  NicaFlowPtVariable();
  virtual Double_t GetVariable(NicaTrack* p);
  virtual NicaFlowVariable* MakeCopy() const {
    return new NicaFlowPtVariable(*this);
  };
  virtual ~NicaFlowPtVariable();
  ClassDef(NicaFlowPtVariable, 1)
};

#endif /* NICAFLOWPTVARIABLE_H_ */
