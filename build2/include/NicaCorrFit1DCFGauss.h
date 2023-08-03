/*
 * NicaCorrFit1DCFGauss.h
 *
 *  Created on: 03-04-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICACORRFIT1DCFGAUSS_H_
#define NICACORRFIT1DCFGAUSS_H_
#include "NicaCorrFit1DCF.h"
/**
 * function for fitting 1-dim CF with gaussian source, taking into account only
 * quantum statistic effect
 */
class NicaCorrFit1DCF_Gauss : public NicaCorrFit1DCF {
 protected:
  Double_t CalculateCF(const Double_t *x, const Double_t *params) const;

 public:
  NicaCorrFit1DCF_Gauss();
  virtual ~NicaCorrFit1DCF_Gauss();
  ClassDef(NicaCorrFit1DCF_Gauss, 1)
};

#endif /* NICACORRFIT1DCFGAUSS_H_ */
