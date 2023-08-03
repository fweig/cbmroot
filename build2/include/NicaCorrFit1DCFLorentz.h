/*
 * NicaCorrFit1DCF_Lorentz.h
 *
 *  Created on: 12 lis 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACORRFIT1DCFLORENTZ_H_
#define NICACORRFIT1DCFLORENTZ_H_

#include "NicaCorrFit1DCF.h"

class NicaCorrFit1DCF_Lorentz : public NicaCorrFit1DCF {
 protected:
  Double_t CalculateCF(const Double_t *x, const Double_t *params) const;

 public:
  NicaCorrFit1DCF_Lorentz();
  virtual ~NicaCorrFit1DCF_Lorentz();
  ClassDef(NicaCorrFit1DCF_Lorentz, 1)
};

#endif /* NICAROOT_ANALYSIS_FEMTO_CORRFIT_FITTINGFUNCTIONS_NICACORRFIT1DCFLORENTZ_H_ \
        */
