/*
 * NicaCorrFit1DCFExp.h
 *
 *  Created on: 29-04-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICACORRFIT1DCF_EXP_H_
#define NICACORRFIT1DCF_EXP_H_

#include "NicaCorrFit1DCF.h"
/**
 * class for fitting CF with exponential source and only quantum statistic taken
 * into account
 */
class NicaCorrFit1DCF_Exp : public NicaCorrFit1DCF {
 protected:
  Double_t CalculateCF(const Double_t *x, const Double_t *params) const;

 public:
  NicaCorrFit1DCF_Exp();
  virtual ~NicaCorrFit1DCF_Exp();
  ClassDef(NicaCorrFit1DCF_Exp, 1)
};

#endif /* NICACORRFIT1DCFEXP_H_ */
