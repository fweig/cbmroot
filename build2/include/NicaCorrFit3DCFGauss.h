/*
 * NicaCorrFit3DCFGauss.h
 *
 *  Created on: 08-04-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICACORRFIT3DCFGAUSS_H_
#define NICACORRFIT3DCFGAUSS_H_
#include "NicaCorrFit3DCF.h"
/**
 * class for fitting simple 3d CF with gaussian shape where only quantum
 * statistic play role
 */
class NicaCorrFit3DCF_Gauss : public NicaCorrFit3DCF {
 protected:
  Double_t CalculateCF(const Double_t *x, const Double_t *params) const;

 public:
  NicaCorrFit3DCF_Gauss();
  virtual ~NicaCorrFit3DCF_Gauss();
  ClassDef(NicaCorrFit3DCF_Gauss, 1)
};

#endif /* NICACORRFIT3DCFGAUSS_H_ */
